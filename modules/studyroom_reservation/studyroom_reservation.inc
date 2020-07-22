<?php

/**
 * @file
 * Contains entity_api class files.
 */

/**
 * The class used for studyroom_reservation entities
 */
class StudyroomReservation extends Entity {
  /**
   * Default constructor.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'studyroom_reservation');
  }

  /**
   * Default defaultLabel.
   */
  protected function defaultLabel() {
    return 'Reservation ' . $this->reservation_id;
  }

  /**
   * Default defaultUri.
   */
  protected function defaultUri() {
    return array('path' => STUDYROOM_RESERVATION_PATH . $this->reservation_id);
  }

  /**
   * Function to determine if reservation is locked.
   */
  public function isLocked() {
    return isset($this->locked) ? $this->locked : 0;
  }

  /**
   * Determine if a conflict exists
   * !!!!WITH 2 hr buffer. Original function comment out below
   */
  public function checkConflict() {
    $conflict = FALSE;
    $reservations = studyroom_reservation_per_space_per_day($this->space_id, $this->start_time->format('Y-m-d'), $this);
    $this->start_time_object = $this->start_time;
    date_default_timezone_set("America/Chicago");
    $this->start_time = $this->start_time->format('U');
    $this->end_time = $this->end_time->format('U');

    foreach ($reservations as $current) {
      if ($current->end_time <= intval($this->start_time)) {
        // This start time is after this reservation end time, so OK.
        continue;
      }
      if ($current->start_time < (intval($this->end_time)+7200)) {
        // End time is past start time of this reservation, so no good.
        $conflict = TRUE;
        $this->conflict_resv_ids[] = $current->reservation_id;
      }
    }

    return $conflict;
  }


//  /**
//   * Determine if a conflict exists.
//   */
//  public function checkConflict() {
//    $conflict = FALSE;
//    $reservations = studyroom_reservation_per_space_per_day($this->space_id, $this->start_time->format('Y-m-d'), $this);
//    $this->start_time_object = $this->start_time;
//    $this->start_time = $this->start_time->format('U');
//    $this->end_time = $this->end_time->format('U');
//
//    foreach ($reservations as $current) {
//      if ($current->end_time <= $this->start_time) {
//        // This start time is after this reservation end time, so OK.
//        continue;
//      }
//      if ($this->end_time > $current->start_time) {
//        // End time is past start time of this reservation, so no good.
//        $conflict = TRUE;
//        $this->conflict_resv_ids[] = $current->reservation_id;
//      }
//    }
//
//    return $conflict;
//  }

  /**
   * Determine if a conflict exists.
   */
  public function checkRuleMaxResvPer($user_name = NULL, $resv_date_array) {
    // get reservation type
    $type = studyroom_reservation_type_load($this->type);

    // don't check for max values if type is "public" or "UI"
    if (!$type->public || !$type->ui) {
      return 0;
    }

    // Load user info
    $account = user_load_by_name($user_name);
    $uid = $account->uid;

    // Load space info
    $space = studyroom_space_load($this->space_id);
    $spaces = studyroom_space_get_spaces_by_type($space->type);
    $space_ids = array_keys($spaces);

    // defaults for the max values
    $max_daily = 1;
    $max_weekly = 7;

    //get max values for space
    $query_studyroom_reservation_type = new EntityFieldQuery();
    $query_studyroom_reservation_type->entityCondition('entity_type', 'studyroom_reservation_type')
      ->entityCondition('entity_type', 'studyroom_reservation_type')
      ->propertyCondition('type', $this->type);
    $results_studyroom_reservation_type = $query_studyroom_reservation_type->execute();
    if (isset($results_studyroom_reservation_type['studyroom_reservation_type'])) {
      $items_nids = array_keys($results_studyroom_reservation_type['studyroom_reservation_type']);
      $items = entity_load('studyroom_reservation_type', $items_nids);

      foreach ($items as $item) {
        $max_daily = $item->max_daily;
        $max_weekly = $item->max_weekly;
      }
    }

    // set defaults to week counts
    $week_count_total = 0;
    $current_week = '';
    $current_weeklyCount = 0;
    $week_check = true;
    $countArray = array();

    // loop through reservations array to check and tally maxes for each week and day.
    foreach($resv_date_array as $resv_date) {
      // Get midnight of first day of week and set week and day starts
      $resv_date_obj = new DateTime($resv_date['value']);
      $selected_day_start = new DateTime($resv_date['value']);
      $weekday = $resv_date_obj->format('w');
      $week_start = $resv_date_obj->sub(new DateInterval('P' . $weekday . 'D'));
      $week_start->setTime(0, 0, 0);
      $weeknumber = $week_start->format('W');
      $dailyCount = 0;

      // looks at current week in the loop and sets variables accordingly. Sets variable to true if it is a new week in regards to previous date in the loop.
      if ($current_week == '' || $current_week != $weeknumber) { // new week
        $current_week = $week_start->format('W');
        $week_count_total = 1;
        $week_check = true;
      } else { // same week, add one to the count for max limit checking
        $week_check = false;
        $week_count_total++;
      }

      // Set day and week starts.
      $selected_day_start->setTime(0, 0, 0);
      $rule_week_start = $week_start->format('Y-m-d H:i:s');
      $rule_day_start = $selected_day_start->format('Y-m-d H:i:s');

      // Set week and day end
      $week_end = $week_start->add(new DateInterval('P7D'));
      $rule_week_end = $week_end->format('Y-m-d H:i:s');
      $day_end = $selected_day_start->add(new DateInterval('P1D'));
      $rule_day_end = $day_end->format('Y-m-d H:i:s');

      // get user's current reservations for the week.
      // Only check once per week sent (if array of dates)
      if ($week_check === true) {
        $query = new EntityFieldQuery();
        $query->entityCondition('entity_type', 'studyroom_reservation')
          ->propertyCondition('type', $this->type)
          ->propertyCondition('space_id', $space_ids, 'IN')
          ->propertyCondition('uid', $uid)
          ->fieldCondition('field_reservation_datetime', 'value', [
            $rule_week_start,
            $rule_week_end
          ], 'BETWEEN')
          ->fieldOrderBy('field_reservation_datetime', 'value', 'ASC');
        $results = $query->execute();
        $resvs = [];
        if (isset($results['studyroom_reservation'])) {
          $resvs = array_keys($results['studyroom_reservation']);
        }
        $weeklyCount = $current_weeklyCount = count($resvs);
      } else { // use previous count since it is the same week. No need to hit the db and check again.
        $weeklyCount = $current_weeklyCount;
      }

      // add new count to existing for current total.
      $weeklyCount = $weeklyCount + $week_count_total;

      // get user's current reservations for the day. Needs to be checked for every date in the array.
      $query_daily = new EntityFieldQuery();
      $query_daily->entityCondition('entity_type', 'studyroom_reservation')
        ->propertyCondition('type', $this->type)
        ->propertyCondition('space_id', $space_ids, 'IN')
        ->propertyCondition('uid', $uid)
        ->fieldCondition('field_reservation_datetime', 'value', [
          $rule_day_start,
          $rule_day_end
        ], 'BETWEEN')
        ->fieldOrderBy('field_reservation_datetime', 'value', 'ASC');
      $results_daily = $query_daily->execute();
      $resvs_daily = [];
      if (isset($results_daily['studyroom_reservation'])) {
        $resvs_daily = array_keys($results_daily['studyroom_reservation']);
      }
      $dailyCount = count($resvs_daily) + 1;

      // push daily count, weekly count, and the date to an array. Will be added to $countArray and returned and analized to check_if_user_max_reached function in studyroom_reservation.admin.inc
      array_push($countArray, array('day'=>$resv_date['value'], 'weekly'=>$weeklyCount, 'daily'=>$dailyCount));
    } // end of foreach($resv_date_array as $resv_date)

    // set up return var with all values
    $countArray['max_daily'] = intval($max_daily);
    $countArray['max_weekly'] = intval($max_weekly);

    // return full array with daily and weekly limits per date in the array as well as the max possible for this reservation type.
    return $countArray;
  }
}

/**
 * The class used for studyroom_reservation type entities
 */
class StudyroomReservationType extends Entity {
  public $type;
  public $label;
  public $locked;

  /**
   * Default constructor.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'studyroom_reservation_type');
  }

  /**
   * Override save function to add default values.
   */
  public function save() {
    if (!isset($this->min_duration)) {
      $this->min_duration = $this->defaultMinDuration();
    }
    if (!isset($this->max_duration)) {
      $this->max_duration = $this->defaultMaxDuration();
    }
    parent::save();
  }

  /**
   * Function to determine if reservation type is locked.
   */
  public function isLocked() {
    return isset($this->locked) ? $this->locked : 0;
  }

  /**
   * Return default value.
   */
  public function defaultMinDuration() {
    return '1800';
  }

  /**
   * Return default value.
   */
  public function defaultMaxDuration() {
    return '14400';
  }

  /**
   * Build array of options for min_duration.
   *
   * @return array
   *   array of options formated for use in select form element
   */
  public function optionsMinDuration() {
    $values = array(
      1800, 3600, 7200, 10800, 14400
    );
    return drupal_map_assoc($values, 'format_interval');
  }

  /**
   * Build array of options for max_duration.
   *
   * @return array
   *   array of options formated for use in select form element
   */
  public function optionsMaxDuration() {
    $values = array(
      1800, 3600, 7200, 10800, 14400, 21600, 32400, 43200, 64800, 86400
    );
    return drupal_map_assoc($values, 'format_interval');
  }
}


/**
 * The Controller for StudyroomReservation entities
 */
class StudyroomReservationController extends EntityAPIController {
  /**
   * Default constructor.
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  /**
   * Create a studyroom_reservation.
   *
   * We first set up the values that are specific to our studyroom_reservation
   * schema but then also go through the EntityAPIController function.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our StudyroomReservation.
    $values += array(
      'reservation_id' => '',
      'is_new' => TRUE,
      'title' => '',
      'created' => '',
      'changed' => '',
    );

    $entity = parent::create($values);
    return $entity;
  }

  /**
   * Delete override.
   *
   * Override delete to prevent deletion of locked types.
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $entities = $ids ? $this->load($ids) : FALSE;
    if (!$entities) {
      // Do nothing, in case invalid or no ids have been passed.
      return;
    }

    foreach ($entities as $id => $entity) {
      if ($entity->locked) {
        drupal_set_message(t('Locked reservation cannot be deleted.'));
      }
      else {
        parent::delete(array($id));

        // If not admin type, notify user of delete.
        if ($entity->type != 'admin') {
          /*
          $entity->uid
          notify_user_hook here? OR use entity_delete in studyroom_user module?
          */
          if (user_access('fake')) {
            drupal_set_message(t('@todo Notify user about delete!'), 'status', FALSE);
          }
        }
      }
    }
  }

  /**
   * Delete locked reservartions.
   */
  public function deleteLocked($ids) {
    parent::delete($ids);
  }

  /**
   * Load override.
   *
   * Allow theming of reservation label for availabilty module.
   */
  public function load($ids = array(), $conditions = array()) {
    $reservations = parent::load($ids, $conditions);
    foreach ($reservations as &$reservation) {
      $function = 'theme_studyroom_reservation_' . $reservation->type . '_label';
      if (function_exists($function)) {
        $reservation->label = $function();
      }
      else {
        if ($reservation->type == 'internal_rooms') {
          $reservation->label = t('Unavailable');
        } else {
          $reservation->label = t('Busy');
        }
      }
    }

    return $reservations;
  }
}

/**
 * The Controller for StudyroomReservation entities.
 */
class StudyroomReservationTypeController extends EntityAPIControllerExportable {
  /**
   * Default constructor.
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  /**
   * Create a studyroom_reservation type.
   *
   * We first set up the values that are specific to our reservation type
   * schema but then also go through the EntityAPIController function.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our StudyroomReservation.
    $values += array(
      'is_new' => TRUE,
      'locked' => 0,
      'public' => 1,
      'module' => 'studyroom_reservation',
    );
    $entity = parent::create($values);
    return $entity;
  }


  /**
   * Save override.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity);

    // Create new instance of field.
    studyroom_reservation_create_field_instances($entity->type);

    // Clear types cache and force menu rebuild.
    studyroom_reservation_types_reset();
    variable_set('menu_rebuild_needed', TRUE);
  }

  /**
   * Delete override.
   *
   * Override delete to prevent deletion of locked types.
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $entities = $ids ? $this->load($ids) : FALSE;
    if (!$entities) {
      // Do nothing, in case invalid or no ids have been passed.
      return;
    }

    foreach ($entities as $entity) {
      if ($entity->locked) {
        drupal_set_message(t('Locked reservation types cannot be deleted.'));
      }
      else {
        parent::delete(array('0' => $entity->type));
      }
    }

    // Clear types cache and force menu rebuild.
    studyroom_reservation_types_reset();
    variable_set('menu_rebuild_needed', TRUE);
  }
}

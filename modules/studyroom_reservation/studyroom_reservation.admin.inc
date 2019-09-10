<?php

/**
 * @file
 * Studyroom Reservation editing UI.
 */

/**
 * UI controller.
 */
class StudyroomReservationUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $id_count = count(explode('/', $this->path)) - 1;
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $defaults = array(
      'file' => $this->entityInfo['admin ui']['file'],
      'file path' => isset($this->entityInfo['admin ui']['file path']) ? $this->entityInfo['admin ui']['file path'] : drupal_get_path('module', $this->entityInfo['module']),
    );

    $items[$this->path] = array(
      'title' => 'Reservations',
      'description' => 'Add, edit and update studyroom reservations.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
      'type' => MENU_LOCAL_TASK,
    );

    $items[$this->path . 'add'] = array(
      'title callback' => 'entity_ui_get_action_title',
      'title arguments' => array('add', $this->entityType),
      'page callback'  => 'studyroom_reservation_add_page',
      'page arguments' => array($this->entityType),
      'access callback'  => 'studyroom_reservation_access',
      'access arguments' => array('create-reservation'),
      'type' => MENU_LOCAL_ACTION,
    ) + $defaults;

    foreach (studyroom_reservation_types() as $type) {
      $items[$this->path . 'add/' . $type->type] = array(
        'title' => 'Add ' . drupal_strtolower($type->label) . ' reservation',
        'page callback' => 'studyroom_reservation_form_wrapper',
        'page arguments' => array(studyroom_reservation_create(array('type' => $type->type))),
        'access callback' => 'studyroom_reservation_access',
        'access arguments' => array('create', $type),
        'description' => $type->description,
      ) + $defaults;
      $items[$this->path . 'add/' . $type->type . '/%/%'] = array(
        'title' => 'Add ' . drupal_strtolower($type->label) . ' reservation',
        'page callback' => 'studyroom_reservation_form_wrapper',
        'page arguments' => array(studyroom_reservation_create(array(
          'type' => $type->type)), 3, 4),
        'access callback' => 'studyroom_reservation_access',
        'access arguments' => array('create', $type),
        'description' => $type->description,
      ) + $defaults;
    }

    $items[$this->path . $wildcard] = array(
      'title callback' => 'entity_ui_get_page_title',
      'title arguments' => array('view', $this->entityType, $id_count),
      'page callback' => 'entity_ui_entity_page_view',
      'page arguments' => array($id_count),
      'load arguments' => array($this->entityType),
      'access callback' => 'studyroom_reservation_access',
      'access arguments' => array('view', $id_count),
    );
    $items[$this->path . $wildcard . '/view'] = array(
      'title' => 'View',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'load arguments' => array($this->entityType),
      'weight' => -10,
    );
    $items[$this->path . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'page callback' => 'studyroom_reservation_form_wrapper',
      'page arguments' => array($id_count),
      'access callback' => 'studyroom_reservation_access',
      'access arguments' => array('edit', $id_count),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'weight' => 0,
    ) + $defaults;
    $items[$this->path . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'studyroom_reservation_delete_form_wrapper',
      'page arguments' => array($id_count),
      'access callback' => 'studyroom_reservation_access',
      'access arguments' => array('delete', $id_count),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
    ) + $defaults;

    return $items;
  }

  /**
   * Function to display add space content.
   *
   * Create the markup for the add StudyroomReservation Entities page
   * within the class so it can easily be extended/overriden.
   */
  public function addPage() {
    $spaces = studyroom_space_get_spaces();
    if (empty($spaces)) {
      drupal_set_message(t('No spaces have been defined yet, so no reservations can be made.'), 'error');
      drupal_goto();
    }

    $item = menu_get_item();
    $content = system_admin_menu_block($item);
    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }

    $types = studyroom_reservation_type_user_access();
    // If user only has access to create one type, redirect to that one.
    if (count($types) == 1) {
      $type = array_shift($types);
      $link = array('add', $type->type, arg(2), arg(3));
      drupal_goto(STUDYROOM_RESERVATION_PATH . implode('/', array_filter($link)));
    }

    $content = theme('studyroom_reservation_choose_type', array(
      'types' => $types, 'space' => arg(2), 'datetime' => arg(3)));
    return $content;
  }
}

/**
 * Page to add StudyroomReservation Entities.
 *
 * @todo Pass this through a proper theme function
 */
function studyroom_reservation_add_page() {
  $controller = entity_ui_controller('studyroom_reservation');
  return $controller->addPage();
}

/**
 * Form callback wrapper: create or edit a reservation.
 *
 * @see studyroom_reservation_edit_form()
 */
function studyroom_reservation_form_wrapper($entity, $space = NULL, $date_time = NULL) {
  return drupal_get_form('studyroom_reservation_edit_form', $entity, $space, $date_time);
}

/**
 * Form callback wrapper: delete a reservation.
 *
 * @see studyroom_studyroom_reservation_delete_form()
 */
function studyroom_reservation_delete_form_wrapper($entity) {
  return drupal_get_form('studyroom_reservation_delete_form', $entity);
}

/**
 * Form callback: create or edit a studyroom_reservation.
 */
function studyroom_reservation_edit_form($form, &$form_state, $entity, $space = NULL, $date_time = NULL) {
  if (isset($entity->id)) {
    drupal_set_title(t('Reservation %id', array('%id' => $entity->id)));
  }
  elseif (!user_access('access reservations overview')) {
    drupal_set_title(t('Create new reservation'));
  }

  $type = isset($entity->type) ? $entity->type : studyroom_reservation_type_default();
  $studyroom_reservation_type = studyroom_reservation_type_load($type);
  if (isset($space) && is_numeric($space)) {
    $studyroom_space = studyroom_space_load($space);
  }
  elseif (isset($entity->space_id)) {
    $studyroom_space = studyroom_space_load($entity->space_id);
  }
  else {
    $studyroom_space = NULL;
  }

  // Get studyroom values.
  $studyroom = new Studyroom();
  $studyroom->entity_type = $studyroom_reservation_type;
  $studyroom->space = $studyroom_space;
  $studyroom->min_duration = $studyroom->getMinDuration($studyroom_space, $studyroom->entity_type);
  $studyroom->max_duration = $studyroom->getMaxDuration($studyroom_space, $studyroom->entity_type);
  $studyroom->max_days_advance = $studyroom->getMaxDaysAdvance($studyroom_space, $studyroom->entity_type);
  
  // Space(s).
  if (!empty($studyroom_space)) {
    $form['space_id_display'] = array(
      '#type' => 'item',
      '#title' => t('Space'),
      '#markup' => $studyroom_space->label(),
    );
    $form['space_id'] = array(
      '#type' => 'hidden',
      '#value' => $studyroom_space->space_id,
    );
  }
  else {
    $spaces = array();
    foreach (studyroom_space_get_spaces() as $space) {
      $spaces[$space->space_id] = $space->label();
    }

    $form['space_id'] = array(
      '#type' => 'select',
      '#title' => t('Space'),
      '#options' => $spaces,
      '#default_value' => isset($entity->space_id) ? $entity->space_id : '',
    );
  }

  $form['duration'] = array(
    '#type' => 'select',
    '#title' => t('Duration'),
    '#options' => drupal_map_assoc($studyroom->durationMinutes(), 'format_interval'),
    '#default_value' => isset($entity->duration) ? $entity->duration : '3600',
  );

  if (isset($entity->uid)) {
    $user_obj = user_load($entity->uid);
    $name = $user_obj->name;
  }
  else {
    // If no uid set, use current user.
    $name = $GLOBALS['user']->name;
  }

  if ($studyroom->entity_type->public) {
    $form['user_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Reservation owner'),
      '#maxlength' => 60,
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => !empty($name) ? $name : 0,
      '#weight' => -1,
      '#required' => TRUE,
      '#access' => user_access("access user profiles"),
      // Need "View user profiles" to use this.
    );
  }
  else {
    $form['user_name'] = array(
      '#type' => 'hidden',
      '#value' => $name,
    );
  }

  // Add the field related form elements.
  // @todo Do I need to add all of these?  studyroom/studyroom_reservation yes.
  $form_state['studyroom'] = $studyroom;
  $form_state['studyroom_reservation'] = $entity;

  field_attach_form('studyroom_reservation', $entity, $form, $form_state);

  // If date passed in, then set default value for datetime field.
  if (isset($date_time)) {
    $date_time = new DateObject($date_time);
    $today = new DateObject();
    $day_diff = $today->difference($date_time, 'days', FALSE);
    if ($day_diff > $studyroom->max_days_advance) {
      $date_time = $today->add(new DateInterval('P' . $studyroom->max_days_advance . 'D'));
      // @todo Better message
      drupal_set_message(t('The date trying to be used is to far in the future.'));
    }

    if ($today->difference($date_time, 'minutes', FALSE) < 0) {
      $date_time = $today;
      drupal_set_message(t('You cannot try to create a reservation in the past.'));
    }

    // Set default date/time for existing date input and multidate input
    $lang = $form['field_reservation_datetime']['#language'];
    $form['field_reservation_datetime'][$lang][0]['#default_value']['value'] = $date_time->format('Y-m-d h:i a');
    $lang_multi = $form['field_multi_reservation_date_tim']['#language'];
    $form['field_multi_reservation_date_tim'][$lang_multi][0]['#default_value']['value'] = $date_time->format('Y-m-d h:i a');
  }

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save reservation'),
    '#submit' => $submit + array('studyroom_reservation_edit_form_submit'),
  );

  $delete_button = TRUE;
  if ($entity->isLocked()) {
    $delete_button = FALSE;
  }
  elseif (isset($entity->is_new)) {
    $delete_button = !$entity->is_new;
  }

  $path = user_access('access reservations overview') ? STUDYROOM_RESERVATION_ADMIN_PATH : 'user/' . $GLOBALS['user']->uid . '/reservations';
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete reservation'),
    '#suffix' => l(t('Cancel'), $path),
    '#submit' => array('studyroom_reservation_form_submit_delete'),
    '#weight' => 45,
    '#access' => $delete_button,
    '#limit_validation_errors' => array(),
  );

  // TODO possibly check time conflicts and show greyed out in time picker. $conflict = $entity->checkConflict();

  // Allow modules to modify the form.
  drupal_alter('studyroom_reservation_edit_form', $form, $form_state);

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'studyroom_reservation_edit_form_validate';
  return $form;
}

/**
 * Form API validate callback for the studyroom_reservation form. Same form if you are adding a new one or editing an existing.
 */
function studyroom_reservation_edit_form_validate(&$form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    drupal_goto(STUDYROOM_RESERVATION_PATH . $form_state['studyroom_reservation']->reservation_id . '/delete');
  }
  // Get form submitted values.
  $values = $form_state['values'];
  $entity = $form_state['studyroom_reservation'];
  $entity->space_id = $values['space_id'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('studyroom_reservation', $entity, $form, $form_state);

  // default values for setting multi-date and the reservations array.
  $multi_date = false;
  $resv_date_array = array();

  // if reservation type is faculty or admin, get reservation date and time from the input with the multi-date option.
  // Even if the user chooses no repeat, use this same input. It will just be a one index array
  // Also check if there is a reservation id passed in. If so, treat as a single date and skip any multi-date input.
  if (($entity->type == 'faculty' || $entity->type == 'admin') && empty($entity->reservation_id)) {
    // gets the variable set in the array so it can be referenced. Usually set to something like 'und'
    $multi_res_lang = $form['field_multi_reservation_date_tim']['#language'];

    // if multi day options were requested, set multi-date to true
    if (isset($values['field_multi_reservation_date_tim'][$multi_res_lang][0]['rrule']) && $values['field_multi_reservation_date_tim'][$multi_res_lang][0]['rrule'] !== NULL) {
      $multi_date = true;
    }

    //set reservation array and timezone.
    $resv_date_array = $values['field_multi_reservation_date_tim'][$multi_res_lang];
    $tz = $form['field_multi_reservation_date_tim'][$multi_res_lang][0]['#date_timezone'];

    // remove last two in the array. They are not related to the dates we need to loop through.
    if ($multi_date === false) {
      array_pop($resv_date_array);
    }
  } else { // reservation type is  not faculty or admin and will not have multi-dates.
    // Treat as a traditional reservation and get the value from the older input, but set it as a single index array so it can be processed the same as multi-date.
    $langcode = $form['field_reservation_datetime']['#language'];
    $resv_date_array = $values['field_reservation_datetime'][$langcode];
    $tz = $form['field_reservation_datetime'][$langcode][0]['#date_timezone'];
  }

  // loop through reservation array (could be multiple or single).
  foreach ($resv_date_array as $resv_date) {
    // Get and set start and end time of reservation
    $entity->start_time = new DateObject($resv_date['value'], $tz, 'Y-m-d H:i:s');
    $entity->end_time = clone $entity->start_time;
    $entity->end_time = $entity->end_time->add(new DateInterval('PT' . $values['duration'] . 'S'));

    // Check if reservation time is outside space/site hours.
    $check_if_in_space_open_hours = check_if_date_within_space_open_hours($form_state['studyroom'], $entity->space_id, $entity, $values['duration'], $tz);
    if ($check_if_in_space_open_hours !== TRUE) { // if error is returned
      form_set_error('', t($check_if_in_space_open_hours));
      return;
    }

    // Check if reservation request before now().
    $today = date_now();
    $check_if_past = check_if_date_is_past($today, $entity->start_time, $entity->type);
    if ($check_if_past !== TRUE) { // if error is returned
      form_set_error('', t($check_if_past));
      return;
    }

    // Check if reservation spans more than one day.
    $check_if_covers_two_days = check_if_date_covers_two_days($entity->start_time, $entity->end_time);
    if ($check_if_covers_two_days !== TRUE) { // if error is returned
      form_set_error('', t($check_if_covers_two_days));
      return;
    }

    // Check if reservation is within the allowed "days out" or "max_days_advance"
    $studyroom = $form_state['studyroom'];
    $check_if_under_max_days_advance = check_if_date_within_max_days_advance($studyroom, $today, $entity->start_time);
    if ($check_if_under_max_days_advance !== TRUE) { // if error is returned
      form_set_error('', t($check_if_under_max_days_advance));
      return;
    }

    // Check if reservation has any existing conflicts
    $check_if_conflict = check_if_reservation_has_conflict($entity, $entity->start_time);
    if ($check_if_conflict !== TRUE) { //if error is returned
      form_set_error('', t($check_if_conflict));
      return;
    }
  }

  // Check if user has reached the max reservations allowed per day/week (only needs to be passed in once. not in loop)
  $check_if_max_reached = check_if_user_max_reached($form_state, $entity, $resv_date_array);
  if ($check_if_max_reached !== TRUE) { //if error is returned
    form_set_error('', t($check_if_max_reached));
    return;
  }
}

/**
 * Check if requested date is in the past. Return error if so.
 */
function check_if_date_is_past($today, $start_time, $type) {
  $interval = $today->difference($start_time, 'minutes', FALSE);
  if ($interval < 0 || !user_access("edit any ".$type." studyroom_reservation")) {
    return 'Your reservation at '.str_replace(' America/Chicago', '', $start_time).' is in the past. Please select a date/time in the future.';
  } else {
    return true;
  }
}

/**
 * Check if requested date covers 2 days (through midnight)
 */
function check_if_date_covers_two_days($start_time, $end_time) {
  if ($start_time->difference($end_time, 'days', FALSE)) {
    if ($end_time->format('H:i:s') != '00:00:00') {
      return 'Your reservation starting at '.str_replace(' America/Chicago', '', $start_time).' cannot span more than one day. Please select an early start time or shorter duration.';
    }
  }
  return true;
}

/**
 * Check if requested date is within the allowed "days out" or "max_days_advance"
 */
function check_if_date_within_max_days_advance($studyroom, $today, $start_time) {
  if (isset($studyroom->max_days_advance)) {
    if ($today->difference($start_time, 'days', FALSE) > $studyroom->max_days_advance) {
      $number_of_days = format_plural($studyroom->max_days_advance, '1 day', '@count days');
      return 'Your reservation at '.str_replace(' America/Chicago', '', $start_time).' is beyond the limit of '.$number_of_days.' in the future.';
    }
  }
  return true;
}

/**
 * Check if requested date is within the space's available open hours
 */
function check_if_date_within_space_open_hours($studyroom, $space_id, $entity, $duration, $tz) {
  if (!isset($studyroom->space)) {
    $studyroom->space = studyroom_space_load($space_id);
  }

  $start_time = new DateObject($entity->start_time->originalTime, $tz, 'Y-m-d H:i:s');
  $end_time = clone $start_time;
  $end_time = $end_time->add(new DateInterval('PT' . $duration . 'S'));

  $space_day_hours = $studyroom->space->hoursPerDate($start_time);

  // if space hours are returned as null/empty, the room is closed.
  if (empty($space_day_hours['start_time']) || empty($space_day_hours['end_time'])) {
    return 'Your reservation starting at ' . $start_time->format('Y-m-d H:i:s') . ' occurs on a day that this room is not open. Please try again.';
  } else { // hours returned, check if reservation is within hours.
    if ($start_time < $space_day_hours['start_time']) {
      $time = $space_day_hours['start_time']->format('h:i a');
      return 'Your reservation starting at ' . $start_time->format('Y-m-d H:i:s') . ' cannot start before ' . $time . ' on this day.';
    }

    if ($end_time > $space_day_hours['end_time']) {
      $time = $space_day_hours['end_time']->format('h:i a');
      return 'Your reservation ending at ' . $end_time->format('Y-m-d H:i:s') . ' cannot end after ' . $time . ' on this day.';
    }
  }

  return true;
}

/**
 * Check if requested date has an existing conflict
 */
function check_if_reservation_has_conflict($entity, $start_time) {
  $conflict = $entity->checkConflict();
  if ($conflict) {
    return 'Your reservation at '.str_replace(' America/Chicago', '', $start_time).' conflicts with another reservation.  Please select a different start time and/or duration.';
  }
  return true;
}

/**
 * Check if user has reached the max reservations allowed per day/week
 */
function check_if_user_max_reached($form_state, $entity, $resv_date_array) {
  // check if username was posted. This is required to go get the users current reservations.
  if (isset($form_state['values']['user_name'])) {
    // get array of maxes. This contains the max per day, max per week for the reservation type as well as the count of the current daily and weekly reservations for the user.
    // Once returned, you can compare and see if any dates are over the max.
    $rule_max_per = $entity->checkRuleMaxResvPer($form_state['values']['user_name'], $resv_date_array);

    // set username to see if it is any special case like gradres, facres, or undres.
    $usernameCheck = $form_state['values']['user_name'];

    // set max variables from $rule_max_per array.
    $max_daily = isset($rule_max_per['max_daily']) && is_int($rule_max_per['max_daily']) ? $rule_max_per['max_daily'] : '';
    $max_weekly = isset($rule_max_per['max_weekly']) && is_int($rule_max_per['max_weekly']) ? $rule_max_per['max_weekly'] : '';

    // if gradres, facres, or undres, do not check for limits
    if ($usernameCheck == 'gradres' || $usernameCheck == 'facres' || $usernameCheck == 'undres') {
      // don't check for limits.
    }
    else {
      // $rule_max_per returns 0 if user is not public or is ui. Don't check limits in this case.
      // Also, if editing the form, need to check differently. Allow users to edit and not hit new limits
      if ($rule_max_per != 0 && $form_state['studyroom_reservation']->reservation_id == '') {
        // New reservation, no need to check max at this point. Carry on as normal
      }
      else { // Editing existing reservation. Get reservation's original time and check for maxes
        $query = db_select('field_data_field_reservation_datetime', 'f')
          ->fields('f', ['field_reservation_datetime_value'])
          ->condition('f.entity_id', $form_state['studyroom_reservation']->reservation_id)
          ->execute();
        $result = $query->fetchObject();
        // if a date time was returned from the db for the user.
        if (isset($result->field_reservation_datetime_value)) {
          $orig_date = $result->field_reservation_datetime_value;
          $dateRequested = $resv_date_array[0]["value"];
          $dateRequestedArray = explode(' ', $dateRequested);
          $orig_dateArray = explode(' ', $orig_date);

          // same day, add one to limits just in case so user can still save a new time for the day
          if ($orig_dateArray[0] == $dateRequestedArray[0]) {
            $max_daily++;
            $max_weekly++;
          }
          else {
            // different day, but still check for the week limit.
            // If it is the same week, add one to the limit to allow the date to be edited. If it is a different week, do NOT add a day
            // requested date week start
            $selected_day_start = new DateTime($dateRequested);
            $weekday = $selected_day_start->format('w');
            $week_start = $selected_day_start->sub(new DateInterval('P' . $weekday . 'D'));
            $week_start->setTime(0, 0, 0);
            $rule_week_start = $week_start->format('Y-m-d H:i:s');
            // original date week start
            $orig_day_start = new DateTime($orig_date);
            $weekday_orig = $orig_day_start->format('w');
            $week_start_orig = $orig_day_start->sub(new DateInterval('P' . $weekday_orig . 'D'));
            $week_start_orig->setTime(0, 0, 0);
            $rule_week_start_orig = $week_start_orig->format('Y-m-d H:i:s');
            // only add to weekly count if it is the same start of the week. If not, keep weekly count as is and calculate as normal
            if ($rule_week_start == $rule_week_start_orig) { //same start of week
              $max_weekly++;
            }
          }
        }
      }

      // set vars for the original max daily and max weekly. These are used in the error messages below since it is possible the max values were altered if editing a reservation.
      $max_daily_orig = isset($rule_max_per['max_daily']) && is_int($rule_max_per['max_daily']) ? $rule_max_per['max_daily'] : '';
      $max_weekly_orig = isset($rule_max_per['max_weekly']) && is_int($rule_max_per['max_weekly']) ? $rule_max_per['max_weekly'] : '';

      //remove max values (last 2 indexes) from array so just dates can be processed
      array_pop($rule_max_per);
      array_pop($rule_max_per);

      // loop through the dates in $rule_max_per. Now in array form, it will break down each day and if the count reaches the max at any day, send an error back to the user.
      foreach ($rule_max_per as $res_date_max) {
        $local_datetime = date('m/d/Y h:iA', strtotime($res_date_max['day']));
        $user_daily = isset($res_date_max['daily']) && is_int($res_date_max['daily']) ? $res_date_max['daily'] : '';
        $user_weekly = isset($res_date_max['weekly']) && is_int($res_date_max['weekly']) ? $res_date_max['weekly'] : '';

        // check for daily limit. deny creation if over limit
        if ($user_daily != '' && $max_daily != '') {
          if ($user_daily > $max_daily) {
            $daily_count = $max_daily_orig;
            return 'In regards to your reservation at '.$local_datetime.', you have reached the daily reservation limit of ' . $daily_count .' for that specific day. To request additional reservations, please email <a href="mailto:libpr@ou.edu">libpr@ou.edu</a>.';
          }
        }

        // check for weekly limit. deny creation if over limit
        if ($user_weekly != '' && $max_weekly != '') {
          if ($user_weekly > $max_weekly) {
            $weekly_count = $max_weekly_orig;
            return 'In regards to your reservation at '.$local_datetime.', you have reached the weekly reservation limit of ' . $weekly_count . ' for that specific week. To request additional reservations, please email <a href="mailto:libpr@ou.edu">libpr@ou.edu</a>.';
          }
        }
      }
    }
  }

  return true;
}

/**
 * Form API submit callback for the studyroom_reservation form. Saves the reservation(s) after submitted.
 *
 * @todo remove hard-coded link
 */
function studyroom_reservation_edit_form_submit(&$form, &$form_state) {
  // get posted values from form submission.
  $values = $form_state['values'];

  // set default values for multi-date flag and reservations array
  $multi_date = false;
  $resv_date_array = array();
  $date = ''; //$date used for redirect after submission'

  // check if this is a reservation edit and set accordingly.
  $edit_reservation = isset($form_state['studyroom_reservation']->reservation_id) && !empty($form_state['studyroom_reservation']->reservation_id) ? true : false;

  // get config of "rules_reservation_email" rule. will be used later to turn off and on the rule as needed.
  $rules_config = rules_config_load('rules_reservation_email');

  // treat as multi-day if it is new and the multi-day input was submitted. Also cannot be a reservation edit.
  if (isset($values['field_multi_reservation_date_tim']) && $edit_reservation === false) {
    // if multi day options was requested
    if (isset($values['field_multi_reservation_date_tim']['und'][0]['rrule']) && $values['field_multi_reservation_date_tim']['und'][0]['rrule'] !== NULL) {
      $multi_date = true;
    }

    // set up reservations array and timezone.
    $resv_date_array = $values['field_multi_reservation_date_tim']['und'];
    $tz = $form['field_multi_reservation_date_tim']['und'][0]['#date_timezone'];

    // if multi-day, remove last index of array. It isn't a date and not needed in the array.
    if ($multi_date === false) {
      array_pop($resv_date_array);
    }

    // set reservation count to 0. Used in week calculation.
    $res_count = 0;

    // loop through reservations
    foreach ($resv_date_array as $resv_date) {
      // Get start and end time.
      $resv_date_string = $resv_date['value'];

      // $date used for redirect. Set to first date in the array
      if ($date === '') {
        $date = new DateObject($resv_date_string);
      }

      //overwrite main date in form to be the current date in the loop. Used in saving the entity as a single date.
      $form_state['values']['field_reservation_datetime']['und'][0]['value'] = $resv_date_string;

      // load/update entity with submitted values.
      $entity = entity_ui_controller('studyroom_reservation')->entityFormSubmitBuildEntity($form, $form_state);

      // Add in created and changed times.
      if ($entity->is_new = isset($entity->is_new) ? $entity->is_new : 0) {
        $entity->created = time();
      }
      $entity->changed = time();

      // Set uid field based on user_name value.
      $account = user_load_by_name($entity->user_name);
      if ($account) {
        $entity->uid = $account->uid;
      }
      else {
        $entity->uid = 0;
      }
      unset($entity->user_name);

      // for the first reservation, allow the email rule to trigger. For any other dates in the loop, turn off the email trigger.
      if ($res_count > 0) {
        // set uuid, nid, reservation_id to NUll and is new to 1. This will ensure each entity in this loop is new.
        // If not done, entity would stay the same and it would only update the same reservation with each date in the loop.
        $entity->uuid->value = NULL;
        $entity->nid->value = NULL;
        $entity->is_new = 1;
        $entity->reservation_id = NULL;

        // set rule to false and save it so an email is not sent
        $rules_config->active = FALSE;
        $rules_config->save();
      } else {
        // set email trigger back to active for normal use.
        $rules_config->active = TRUE;
        $rules_config->save();
      }

      // Save the entity (reservation)
      $entity->save();
      $res_count++;
    }
  } else { // no multi-date available for reservation type
    $entity = entity_ui_controller('studyroom_reservation')->entityFormSubmitBuildEntity($form, $form_state);
    // Add in created and changed times.
    if ($entity->is_new = isset($entity->is_new) ? $entity->is_new : 0) {
      $entity->created = time();
    }
    $entity->changed = time();

    // Set uid field based on user_name value.
    $account = user_load_by_name($entity->user_name);
    if ($account) {
      $entity->uid = $account->uid;
    }
    else {
      $entity->uid = 0;
    }
    unset($entity->user_name);

    // save the entity (reservation)
    $entity->save();

    // Get year/week to redirect to correct week of reservation.
    $date = new DateObject($entity->start_time);
  }

  // get url arguments for redirect.
  $args = explode('/', drupal_get_path_alias());
  $location = $form_state['studyroom']->space->type;
  // if this comes from the interactive map page and has the word 'image' in
  // the url we want to redirect back to the interactive map page
  // @todo better handling of the location of the word image
  if (strpos_arr($args, 'image')){
    $url = array($location, 'airport', $args[6]);
    $form_state['redirect'] = 'availability/' . implode('/', $url);
  }
  else if(in_array('day', $args)) {
    $url = array($entity->space_id, 'day');
    $form_state['redirect'] = STUDYROOM_SPACE_PATH . implode('/', $url);
    if ($location == 'bizzell_consultation_room') {
      cas_logout();
    }
  }
  else {
    $url = array($entity->space_id, $date->format('Y'), date_week($date));
    $form_state['redirect'] = STUDYROOM_SPACE_PATH . implode('/', $url);
  }
}

/**
 * @param $haystack
 * @param $needle
 * @return bool|int
 *
 * check if the array $needle is anywhere in the array $haystack
 */
function strpos_arr($haystack, $needle) {
  if(!is_array($needle)) $needle = array($needle);
  $haystack = implode(',', $haystack);
  foreach($needle as $what) {
    if(($pos = strpos($haystack, $what))!==false) return $pos;
  }
  return false;
}

/**
 * Form API submit callback for the delete button.
 *
 * Unset destination to fix views destination issue.
 */
function studyroom_reservation_form_submit_delete(&$form, &$form_state) {
  if (isset($_GET['destination'])) {
    unset($_GET['destination']);
  }
  $form_state['redirect'] = STUDYROOM_RESERVATION_PATH . $form_state['studyroom_reservation']->reservation_id . '/delete';
}

/**
 * Form callback: confirmation form for deleting a reservation.
 *
 * @see confirm_form()
 */
function studyroom_reservation_delete_form($form, &$form_state, $entity) {
  if ($entity->isLocked()) {
    drupal_set_message(t('Reservation %id is locked and can not be deleted.', array('%id' => $entity->reservation_id)), 'error');
    drupal_goto(STUDYROOM_RESERVATION_ADMIN_PATH);
  }

  $form_state['studyroom_reservation'] = $entity;
  $form['#submit'][] = 'studyroom_reservation_delete_form_submit';

  $path = user_access('access reservations overview') ? STUDYROOM_RESERVATION_ADMIN_PATH : 'user/' . $GLOBALS['user']->uid . '/reservations';
  $form_state['studyroom_redirect_path'] = $path;

  $form = confirm_form($form,
    t('Are you sure you want to delete reservation %id?', array('%id' => $entity->reservation_id)),
    $path,
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for studyroom_reservation_delete_form.
 */
function studyroom_reservation_delete_form_submit($form, &$form_state) {
  $entity = $form_state['studyroom_reservation'];
  $entity->delete();

  drupal_set_message(t('Reservation %id has been deleted.', array('%id' => $entity->reservation_id)));
  watchdog('studyroom_reservation', 'Deleted reservation %id.', array('%id' => $entity->reservation_id));

  $form_state['redirect'] = $form_state['studyroom_redirect_path'];
}

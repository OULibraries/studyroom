<?php

/**
 * @file
 * Definition of studyroom_reservation_handler_filter_space.
 */

/**
 * Contains a field filter handler to list all the spaces.
 */
class studyroom_reservation_handler_filter_space extends
  views_handler_filter_in_operator {
  /**
   * Display a list of available spaces in the filter's options.
   */
  public function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Room Name');
      $this->value_options = studyroom_reservation_spaces_options(FALSE);
    }
  }
}

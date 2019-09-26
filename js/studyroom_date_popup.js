/**
 * @file
 * JavaScript for studyroom to modify date_popup time range.
 */

(function ($) {
  Drupal.behaviors.studyrooms = {
    attach: function (context) {
      for (var id in Drupal.settings.datePopup) {
        if (Drupal.settings.datePopup.hasOwnProperty(id)) {
          $('#' + id).bind('focus', Drupal.settings.datePopup[id], function (e) {
            var datePopup = e.data;
            if ($(this).hasClass('date-popup-init')) {
              if (datePopup.func === 'timepicker') {
                //console.log(datePopup.settings);
                $(this).timepicker(datePopup.settings);
                $(this).click(function () {
                  $(this).focus();
                });
              }
              else if (datePopup.func === 'timeEntry') {
                //console.log(datePopup.settings);
                $(this).timeEntry(datePopup.settings);
                $(this).click(function () {
                  $(this).focus();
                });
              }
            }
          });
        }
      }
    }
  };

  jQuery(document).ready(function () {
    // hide the yearly option in the repeat date pop up for room res.
    $("#edit-field-multi-reservation-date-tim-und-0-rrule-freq option[value='YEARLY']").remove();

    // hide and show duration if all day checkbox is checked or not. This one is for the edit page. The click event is for either page when the checkbox is checked or unchecked.
    if ($('#edit-field-all-day input[type="checkbox"]').prop("checked") == true) {
      $('.form-item-duration').hide();
    }

    $('#edit-field-all-day input[type="checkbox"]').click(function() {
      if ($(this).prop("checked") == true) {
        $('.form-item-duration').hide();
      } else if($(this).prop("checked") == false) {
        $('.form-item-duration').show();
      }
    });

    if ($('#edit-space-id').val()) {
      var multi_day_location_list = [21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44];

      var is_faculty_or_admin = $('body').hasClass('page-reservation-add-admin') || $('body').hasClass('page-reservation-add-faculty') ? 1 : 0;

      if (is_faculty_or_admin === 1) {
        var space_id = parseInt($('#edit-space-id').val());

        if (multi_day_location_list.includes(space_id)) {
          $('div.field-name-field-multi-reservation-date-tim').show();
          $('div.field-name-field-reservation-datetime').hide();
          $('#edit-field-all-day').show();
        }
        else {
          $('div.field-name-field-multi-reservation-date-tim').hide();
          $('div.field-name-field-reservation-datetime').show();
          $('#edit-field-all-day').hide();
          $('#edit-field-all-day input[type="checkbox"]').prop("checked", false);
        }

        $('#edit-space-id').on('change', function () {
          space_id = parseInt($('#edit-space-id').val());
          if (multi_day_location_list.includes(space_id)) {
            $('div.field-name-field-multi-reservation-date-tim').show();
            $('div.field-name-field-reservation-datetime').hide();
            $('#edit-field-all-day').show();
          } else {
            $('div.field-name-field-multi-reservation-date-tim').hide();
            $('div.field-name-field-reservation-datetime').show();
            $('#edit-field-all-day').hide();
            $('#edit-field-all-day input[type="checkbox"]').prop("checked", false);
          }
        });
      } else {
        $('div.field-name-field-multi-reservation-date-tim').hide();
        $('div.field-name-field-reservation-datetime').show();
        $('#edit-field-all-day').hide();
        $('#edit-field-all-day input[type="checkbox"]').prop("checked", false);
      }
    }
  });
})(jQuery);
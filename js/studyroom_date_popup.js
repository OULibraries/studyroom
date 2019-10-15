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
    $("#edit-field-multi-reservation-date-tim-und-0-rrule-freq option[value='YEARLY']").remove();
    $("#edit-field-multi-reservation-date-tim-und-0-rrule-freq option[value='MONTHLY']").remove();
    $("#edit-field-multi-reservation-date-tim-und-0-rrule-freq option[value='WEEKLY ']").remove();
    $("#edit-field-multi-reservation-date-tim-und-0-rrule-freq").val('DAILY');
    $("#edit-field-multi-reservation-date-tim-und-0-rrule-freq").prop('disabled', true);

    // hide and show duration if all day checkbox is checked or not. This one is for the edit page. The click event is for either page when the checkbox is checked or unchecked.
    if ($('#edit-field-all-day input[type="checkbox"]').prop("checked") == true) {
      $('.form-item-duration').hide();
      $("#edit-field-multi-reservation-date-tim-und-0-rrule-freq option[value='YEARLY']").remove();
      $("#edit-field-multi-reservation-date-tim-und-0-rrule-freq option[value='MONTHLY']").remove();
      $("#edit-field-multi-reservation-date-tim-und-0-rrule-freq option[value='WEEKLY ']").remove();
      $("#edit-field-multi-reservation-date-tim-und-0-rrule-freq").val('DAILY');
      $("#edit-field-multi-reservation-date-tim-und-0-rrule-freq").prop('disabled', true);
    }

    $('#edit-field-all-day input[type="checkbox"]').click(function() {
      if ($(this).prop("checked") == true) {
        $('.form-item-duration').hide();
      } else if($(this).prop("checked") == false) {
        $('.form-item-duration').show();
      }
    });

    if ($('#edit-space-id').val()) {
      var is_faculty_or_admin = $('body').hasClass('page-reservation-add-admin') || $('body').hasClass('page-reservation-add-faculty') ? 1 : 0;

      if (is_faculty_or_admin === 1) {
        var space_name = $('#edit-space-id option:selected').text();
        var is_longterm_room = space_name.includes('Longterm');

        if (is_longterm_room) {
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
          var space_name = $('#edit-space-id option:selected').text();
          var is_longterm_room = space_name.includes('Longterm');

          if (is_longterm_room) {
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
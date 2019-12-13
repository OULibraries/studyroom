#!/bin/bash
## Run export for past room reservations and clear them

echo "Exporting reservation data..."

## Set SITENAME based on first param provided
SITENAME="libraries"

## set DRUSH PATH and change to the site's drupal dir
cd /srv/$SITENAME/drupal || exit

## Set export file location and name
RESFILE="/var/log/roomres/${SITENAME}_roomres_${HOSTNAME}_output-$(date "+%Y%m%d_%I:%M:%S").csv"

## Run drush command to export query results to a csv file in /var/log/roomres
echo "reservation_id, reservation_type, space_id, user_id, duration, created, deleted, reservation_datetime, space_type, label" > "$RESFILE" | drush sql-query "SELECT r.reservation_id, r.type AS reservation_type, r.space_id, r.uid user_id, r.duration, r.created, rd.deleted, rd.field_reservation_datetime_value reservation_datetime, s.type, s.label FROM studyroom_reservation r LEFT JOIN field_data_field_reservation_datetime rd ON rd.entity_id = r.reservation_id LEFT JOIN studyroom_space s ON s.space_id = r.space_id WHERE rd.field_reservation_datetime_value < date_sub(now(), interval 1 month)" | awk 'BEGIN { FS = "\t"} ; {print "\""$1"\",\""$2"\",\""$3"\",\""$4"\",\""$5"\",\""$6"\",\""$7"\",\""$8"\",\""$9"\",\""$10"\""}' >> "$RESFILE"

echo "Data exported to ${RESFILE}"

echo "Verify csv file exists..."

if read -r && read -r
then
  echo "$RESFILE exists and is more than 1 line"

    echo "Delete data from db..."

    ## send csv file name to function in studyroom_reservation.admin.inc to remove the nodes.
    drush php-eval "module_load_include('inc', 'studyroom_reservation', 'studyroom_reservation.admin'); studyroom_reservation_delete_by_csv('${RESFILE}');"

    echo "Data deleted."
else
  echo "Reservations csv file not properly exported, attention required!"
  exit 1;
fi < "$RESFILE"

echo "Reservation export and deletion complete."

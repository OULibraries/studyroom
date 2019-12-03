#!/bin/bash
## Run export for past room reservations and clear them

## check for first param. Exit if not given.
if [  -z "$1" ]; then
  cat <<USAGE
This script runs an export for past room reservations and removes them form the db.
Usage: roomres_export.sh \$SITENAME

\$SITENAME  Name of the site dir in the /srv directory. Must be a libraries site with reservations.
USAGE

  exit 1;
fi

echo "Exporting reservation data..."

## Set SITENAME based on first param provided
SITENAME=$1

## set DRUSH PATH and change to the site's drupal dir
DRUSH='/opt/php/bin/drush'
cd /srv/$SITENAME/drupal

## Set export file location and name
RESFILE="/var/log/roomres/roomres_${HOSTNAME}_output-$(date "+%Y%m%d_%I:%M:%S").csv"

## Run drush command to export query results to a csv file in /var/log/roomres
echo "reservation_id, reservation_type, space_id, user_id, duration, created, deleted, reservation_datetime, space_type, label" > $RESFILE | drush sql-query "SELECT r.reservation_id, r.type AS reservation_type, r.space_id, r.uid user_id, r.duration, r.created, rd.deleted, rd.field_reservation_datetime_value reservation_datetime, s.type, s.label FROM studyroom_reservation r LEFT JOIN field_data_field_reservation_datetime rd ON rd.entity_id = r.reservation_id LEFT JOIN studyroom_space s ON s.space_id = r.space_id WHERE rd.field_reservation_datetime_value < date_sub(now(), interval 1 month)" | awk 'BEGIN { FS = "\t"} ; {print "\""$1"\",\""$2"\",\""$3"\",\""$4"\",\""$5"\",\""$6"\",\""$7"\",\""$8"\",\""$9"\",\""$10"\""}' >> $RESFILE

echo "Data exported to ${RESFILE}"

echo "Verify csv file exists..."
if test -f "$RESFILE"; then
    echo "$RESFILE exists"

    echo "Delete data from db..."

    ## Query data again to remove from db.
    drush sql-query "DELETE A, B FROM field_data_field_reservation_datetime A INNER JOIN studyroom_reservation B ON  A.entity_id = B.reservation_id WHERE B.reservation_id IN (SELECT rd.entity_id FROM field_data_field_reservation_datetime rd WHERE rd.field_reservation_datetime_value < date_sub(now(), interval 1 month))"

    echo "Data deleted."
else
  echo "Reservations csv file not properly exported, attention required!"
  exit 1;
fi

echo "Reservation export and deletion complete."
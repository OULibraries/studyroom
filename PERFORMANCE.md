# Performance Notes

The studyroom module may have performance issues when scaled past a certain number of rooms and/or users. In our experience, this showed up as irregular performance spikes and database deadlocks. 

We've found that creating the following indexes can improve these issues significantly. 

```
db_add_index('studyroom_reservation', 'idx_space_id', array('space_id'));
db_add_index('field_data_field_reservation_datetime', 'idx_field_reservation_datetime_value', array('field_reservation_datetime_value'));
db_add_index('field_data_field_reservation_datetime', 'idx_reservation_datetime_query', array('entity_type','entity_id','revision_id','bundle'));
db_add_index('field_data_field_reservation_datetime', 'idx_reservation_datetime_entity', array('entity_type','entity_id'))
```

You can use `drush php-script` to run the above php commands against a running site. 

You may find that you need to kill individual processes in MySQL so that index creation isn't blocked. 


/* update ride field formats */

UPDATE field_data_field_ride_description SET field_ride_description_format = 5 WHERE field_ride_description_format IS NULL;
UPDATE field_data_field_ride_speed SET field_ride_speed_format = 7 WHERE field_ride_speed_format IS NULL;
UPDATE field_data_field_ride_token SET field_ride_token_format = 7 WHERE field_ride_token_format IS NULL;
UPDATE field_data_field_ride_start_location SET field_ride_start_location_format = 7 WHERE field_ride_start_location_format IS NULL;
UPDATE field_data_field_ride_distance_in_miles SET field_ride_distance_in_miles_format = 7 WHERE field_ride_distance_in_miles_format IS NULL;

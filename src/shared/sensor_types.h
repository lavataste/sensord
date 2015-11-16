#ifndef _SENSOR_TYPES_H_
#define _SENSOR_TYPES_H_

#ifdef __cplusplus
extern "C"
{
#endif

// Accelerometer
enum accelerometer_event_type {
	ACCELEROMETER_RAW_DATA_EVENT					= (ACCELEROMETER_SENSOR << 16) | 0x0001,
	ACCELEROMETER_UNPROCESSED_DATA_EVENT			= (ACCELEROMETER_SENSOR << 16) | 0x0002,
};

// Gyroscope
enum gyro_event_type {
	GYROSCOPE_RAW_DATA_EVENT	= (GYROSCOPE_SENSOR << 16) | 0x0001,
	GYROSCOPE_UNPROCESSED_DATA_EVENT	= (GYROSCOPE_SENSOR << 16) | 0x0002,
};

// Geomagnetic
enum geomag_event_type {
	GEOMAGNETIC_RAW_DATA_EVENT	= (GEOMAGNETIC_SENSOR << 16) | 0x0001,
	GEOMAGNETIC_CALIBRATION_NEEDED_EVENT	= (GEOMAGNETIC_SENSOR << 16) | 0x0002,
	GEOMAGNETIC_UNPROCESSED_DATA_EVENT	= (GEOMAGNETIC_SENSOR << 16) | 0x0003,
};

// Proximity
enum proxi_event_type {
	PROXIMITY_CHANGE_STATE_EVENT	= (PROXIMITY_SENSOR << 16) | 0x0001,
	PROXIMITY_STATE_EVENT	= (PROXIMITY_SENSOR << 16) | 0x0002,
	PROXIMITY_DISTANCE_DATA_EVENT	= (PROXIMITY_SENSOR << 16) | 0x0004,
};

enum proxi_change_state {
	PROXIMITY_STATE_FAR	= 0,
	PROXIMITY_STATE_NEAR	= 1,
};

enum proxi_property_id {
	PROXIMITY_PROPERTY_UNKNOWN = 0,
};

// Pressure
enum pressure_event_type {
	PRESSURE_RAW_DATA_EVENT 	= (PRESSURE_SENSOR << 16) | 0x0001,
};

// Temperature
enum temperature_event_type {
	TEMPERATURE_RAW_DATA_EVENT	= (TEMPERATURE_SENSOR << 16) | 0x0001,
};

// Light
enum light_event_type {
	LIGHT_LUX_DATA_EVENT	= (LIGHT_SENSOR << 16) | 0x0001,
	LIGHT_LEVEL_DATA_EVENT	= (LIGHT_SENSOR << 16) | 0x0002,
	LIGHT_CHANGE_LEVEL_EVENT	= (LIGHT_SENSOR << 16) | 0x0004,
};

enum light_property_id {
	LIGHT_PROPERTY_UNKNOWN = 0,
};

// Rotation Vector
enum rot_event_type {
	ROTATION_VECTOR_RAW_DATA_EVENT	= (ROTATION_VECTOR_SENSOR << 16) | 0x0001,
};

// Rotation Vector Raw
enum rv_raw_event_type {
	RV_RAW_RAW_DATA_EVENT	= (RV_RAW_SENSOR << 16) | 0x0001,
	RV_RAW_CALIBRATION_NEEDED_EVENT	= (RV_RAW_SENSOR << 16) | 0x0002
};


// Ultraviolet
enum ultraviolet_event_type {
	ULTRAVIOLET_RAW_DATA_EVENT 	= (ULTRAVIOLET_SENSOR << 16) | 0x0001,
};

// Auto rotation
enum auto_rotation_event_type {
	AUTO_ROTATION_CHANGE_STATE_EVENT = (AUTO_ROTATION_SENSOR << 16) | 0x0001,
};

enum auto_rotation_state {
	AUTO_ROTATION_DEGREE_UNKNOWN = 0,
	AUTO_ROTATION_DEGREE_0,
	AUTO_ROTATION_DEGREE_90,
	AUTO_ROTATION_DEGREE_180,
	AUTO_ROTATION_DEGREE_270,
};

// Bio LED Red
enum bio_led_green_event_type {
	BIO_LED_RED_RAW_DATA_EVENT	= (BIO_LED_RED_SENSOR << 16) | 0x0001,
};

// Gaming RV
enum gaming_rv_event_type {
	GAMING_RV_RAW_DATA_EVENT	= (GAMING_RV_SENSOR << 16) | 0x0001,
};

// Context
enum context_event_type {
	CONTEXT_REPORT_EVENT	= (CONTEXT_SENSOR << 16) | 0x0001,
};

enum context_property_id {
	CONTEXT_PROPERTY_UNKNOWN	= 0,
};

// Geomagnetic RV
enum geomagnetic_rv_event_type {
	GEOMAGNETIC_RV_RAW_DATA_EVENT	= (GEOMAGNETIC_RV_SENSOR << 16) | 0x0001,
};

// Gravity
enum gravity_event_type {
	GRAVITY_RAW_DATA_EVENT	= (GRAVITY_SENSOR << 16) | 0x0001,
};

// Linear Acceleration
enum linear_accel_event_type {
	LINEAR_ACCEL_RAW_DATA_EVENT	= (LINEAR_ACCEL_SENSOR << 16) | 0x0001,
};

// Motion
enum motion_event_type {
	MOTION_ENGINE_EVENT_SNAP				= (MOTION_SENSOR << 16) | 0x0001,
	MOTION_ENGINE_EVENT_SHAKE				= (MOTION_SENSOR << 16) | 0x0002,
	MOTION_ENGINE_EVENT_DOUBLETAP			= (MOTION_SENSOR << 16) | 0x0004,
	MOTION_ENGINE_EVENT_PANNING				= (MOTION_SENSOR << 16) | 0x0008,
	MOTION_ENGINE_EVENT_TOP_TO_BOTTOM		= (MOTION_SENSOR << 16) | 0x0010,
	MOTION_ENGINE_EVENT_DIRECT_CALL			= (MOTION_SENSOR << 16) | 0x0020,
	MOTION_ENGINE_EVENT_TILT_TO_UNLOCK		= (MOTION_SENSOR << 16) | 0x0040,
	MOTION_ENGINE_EVENT_LOCK_EXECUTE_CAMERA = (MOTION_SENSOR << 16) | 0x0080,
	MOTION_ENGINE_EVENT_SMART_ALERT			= (MOTION_SENSOR << 16) | 0x0100,
	MOTION_ENGINE_EVENT_TILT				= (MOTION_SENSOR << 16) | 0x0200,
	MOTION_ENGINE_EVENT_PANNING_BROWSE		= (MOTION_SENSOR << 16) | 0x0400,
	MOTION_ENGINE_EVENT_NO_MOVE				= (MOTION_SENSOR << 16) | 0x0800,
	MOTION_ENGINE_EVENT_SHAKE_ALWAYS_ON     = (MOTION_SENSOR << 16) | 0x1000,
	MOTION_ENGINE_EVENT_SMART_RELAY         = (MOTION_SENSOR << 16) | 0x2000,
};

enum motion_snap_event {
	MOTION_ENGIEN_SNAP_NONE			= 0,
	MOTION_ENGIEN_NEGATIVE_SNAP_X	= 1,
	MOTION_ENGIEN_POSITIVE_SNAP_X	= 2,
	MOTION_ENGIEN_NEGATIVE_SNAP_Y	= 3,
	MOTION_ENGIEN_POSITIVE_SNAP_Y	= 4,
	MOTION_ENGIEN_NEGATIVE_SNAP_Z	= 5,
	MOTION_ENGIEN_POSITIVE_SNAP_Z	= 6,
	MOTION_ENGIEN_SNAP_LEFT			= MOTION_ENGIEN_NEGATIVE_SNAP_X,
	MOTION_ENGIEN_SNAP_RIGHT		= MOTION_ENGIEN_POSITIVE_SNAP_X,
	MOTION_ENGINE_SNAP_NONE			= 0,
	MOTION_ENGINE_NEGATIVE_SNAP_X	= 1,
	MOTION_ENGINE_POSITIVE_SNAP_X	= 2,
	MOTION_ENGINE_NEGATIVE_SNAP_Y	= 3,
	MOTION_ENGINE_POSITIVE_SNAP_Y	= 4,
	MOTION_ENGINE_NEGATIVE_SNAP_Z	= 5,
	MOTION_ENGINE_POSITIVE_SNAP_Z	= 6,
	MOTION_ENGINE_SNAP_LEFT			= MOTION_ENGINE_NEGATIVE_SNAP_X,
	MOTION_ENGINE_SNAP_RIGHT		= MOTION_ENGINE_POSITIVE_SNAP_X,
};

enum motion_shake_event {
	MOTION_ENGIEN_SHAKE_NONE		= 0,
	MOTION_ENGIEN_SHAKE_DETECTION	= 1,
	MOTION_ENGIEN_SHAKE_CONTINUING	= 2,
	MOTION_ENGIEN_SHAKE_FINISH		= 3,
	MOTION_ENGINE_SHAKE_BREAK		= 4,
	MOTION_ENGINE_SHAKE_NONE		= 0,
	MOTION_ENGINE_SHAKE_DETECTION	= 1,
	MOTION_ENGINE_SHAKE_CONTINUING	= 2,
	MOTION_ENGINE_SHAKE_FINISH		= 3,
};

enum motion_doubletap_event {
	MOTION_ENGIEN_DOUBLTAP_NONE			= 0,
	MOTION_ENGIEN_DOUBLTAP_DETECTION	= 1,
	MOTION_ENGINE_DOUBLTAP_NONE			= 0,
	MOTION_ENGINE_DOUBLTAP_DETECTION	= 1,
};

enum motion_top_to_bottom_event {
	MOTION_ENGIEN_TOP_TO_BOTTOM_NONE		= 0,
	MOTION_ENGIEN_TOP_TO_BOTTOM_WAIT		= 1,
	MOTION_ENGIEN_TOP_TO_BOTTOM_DETECTION	= 2,
	MOTION_ENGINE_TOP_TO_BOTTOM_NONE		= 0,
	MOTION_ENGINE_TOP_TO_BOTTOM_WAIT		= 1,
	MOTION_ENGINE_TOP_TO_BOTTOM_DETECTION	= 2,
};

enum motion_direct_call_event_t {
	MOTION_ENGINE_DIRECT_CALL_NONE,
	MOTION_ENGINE_DIRECT_CALL_DETECTION,
};

enum motion_smart_relay_event_t {
	MOTION_ENGINE_SMART_RELAY_NONE,
	MOTION_ENGINE_SMART_RELAY_DETECTION,
};

enum motion_tilt_to_unlock_event_t {
	MOTION_ENGINE_TILT_TO_UNLOCK_NONE,
	MOTION_ENGINE_TILT_TO_UNLOCK_DETECTION,
};

enum motion_lock_execute_camera_event_t {
	MOTION_ENGINE_LOCK_EXECUTE_CAMERA_NONE,
	MOTION_ENGINE_LOCK_EXECUTE_CAMERA_L_DETECTION,
	MOTION_ENGINE_LOCK_EXECUTE_CAMERA_R_DETECTION,
};

enum motion_smart_alert_t {
	MOTION_ENGINE_SMART_ALERT_NONE,
	MOTION_ENGINE_SMART_ALERT_DETECTION,
};

enum motion_no_move_t {
	MOTION_ENGINE_NO_MOVE_NONE,
    MOTION_ENGINE_NO_MOVE_DETECTION,
};

enum motion_property_id {
	MOTION_PROPERTY_UNKNOWN = 0,
	MOTION_PROPERTY_CHECK_ACCEL_SENSOR,
	MOTION_PROPERTY_CHECK_GYRO_SENSOR,
	MOTION_PROPERTY_CHECK_GEO_SENSOR,
	MOTION_PROPERTY_CHECK_PRIXI_SENSOR,
	MOTION_PROPERTY_CHECK_LIGHT_SENSOR,
	MOTION_PROPERTY_CHECK_BARO_SENSOR,
	MOTION_PROPERTY_LCD_TOUCH_ON,
	MOTION_PROPERTY_LCD_TOUCH_OFF,
	MOTION_PROPERTY_CHECK_GYRO_CAL_STATUS,
};

// Orientation
enum orientation_event_type {
	ORIENTATION_RAW_DATA_EVENT	= (ORIENTATION_SENSOR << 16) | 0x0001,
	ORIENTATION_CALIBRATION_NEEDED_EVENT	= (ORIENTATION_SENSOR << 16) | 0x0002,
};

// Tilt
enum tilt_event_type {
	TILT_RAW_DATA_EVENT 	= (TILT_SENSOR << 16) | 0x0001,
};

// Uncalibrated Gyroscope
enum uncal_gyro_event_type {
	UNCAL_GYRO_RAW_DATA_EVENT	= (UNCAL_GYROSCOPE_SENSOR << 16) | 0x0001,
};

// Fusion
enum fusion_event_type {
	FUSION_EVENT = (FUSION_SENSOR << 16) | 0x0001,
	FUSION_UNCAL_GYRO_EVENT = (FUSION_SENSOR << 16) | 0x0002,
	FUSION_CALIBRATION_NEEDED_EVENT = (FUSION_SENSOR << 16) | 0x0003,
	FUSION_ORIENTATION_ENABLED = (FUSION_SENSOR << 16) | 0x0004,
	FUSION_ROTATION_VECTOR_ENABLED = (FUSION_SENSOR << 16) | 0x0005,
	FUSION_GAMING_ROTATION_VECTOR_ENABLED = (FUSION_SENSOR << 16) | 0x0006,
	FUSION_GEOMAGNETIC_ROTATION_VECTOR_ENABLED = (FUSION_SENSOR << 16) | 0x0007,
	FUSION_TILT_ENABLED = (FUSION_SENSOR << 16) | 0x0008,
	FUSION_UNCAL_GYRO_ENABLED = (FUSION_SENSOR << 16) | 0x0009,
};


#ifdef __cplusplus
}
#endif

#endif /* _SENSOR_TYPES_H_ */

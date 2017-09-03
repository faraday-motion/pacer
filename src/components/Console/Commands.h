// Dev Tools

// IDEA:: Have the first three digets be unique for each FMV instalation.
// e.g. 3251, 3252, 3253 [..]. This way commands are unique for vehicle.
// TODO:: Figure out how does the the mobile/web app get the correct prefix safely!
enum {
  LOGGER_ENABLE         = 8001,
  LOGGER_DISABLE        = 8002,
  LOGGER_ADD_TARGET     = 8003,
  LOGGER_REMOVE_TARGET  = 8004,
  LOGGER_SET_LEVEL      = 8005,

  CONFIG_GET            = 8006,
  CONFIG_SET            = 8007,
};

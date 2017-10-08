// Radio Communication Commands.h
enum {
  REQUEST_NAME           = 1,  // FMV requests name.
  SAVE_NAME              = 5,  // CTRL sends name.
  // FMV and CTRL proceed to address change.
  
  SAVE_ADDRESS           = 10, // FMV sends new address.
  CONFIRM_SAVE_ADDRESS   = 15, // CTRL confirms reception of address. FMV validates addreess
  CHANGE_ADDRESS         = 20, // FMB requests change address.
  CONFIRM_CHANGE_ADDRESS = 25, // CTRL confirms the change will happen now.
  // FMV and CTRL have changed addresses.

  SAVE_CAHNNEL           = 30, // FMV sends new channel.
  CONFIRM_SAVE_CHANNEL   = 35, // CTRL confirms reception of new channel.FMV validates the channel
  CHANGE_CHANNEL         = 40, // FMV request change channel.
  CONFIRM_CHANGE_CHANNEL = 45, // CTRL confirms the change will happen now.
  // FMV and CTRL change channels.

  WAIT_REGISTER          = 50, // FMV requests CTRL to wait until it is registered.
  CONFIRM_WAIT_REGISTER  = 55, // CTRL confirms waiting to register. FMV tryies to register.

  // if registration fails.
  FAILED_REGISTER         = 60, // FMV sends reason for not registering.
  // if registration succeeds.
  SET_MODE_IDLE           = 65, // FMV requests CTRL to go into IDLE mode. (no more scanning.)
  CONFIRM_SET_MODE_IDLE   = 70, // CTRL confirm that it's in IDEL mode.

  SET_MODE_ACTIVE         = 75, // FMV requests CTRL input.
  CONFIRM_SET_MODE_ACTIVE = 80, // CTRL confirms it's ready to send inputs.
  // CTRL WAITS FOR REQUEST_INPUT

  REQUEST_INPUT           = 85, // FMV request CTRL input.
  CTRL_HAS_INPUT          = 90, // CTRL sends input values.

};

/*
 * Controller strategy
 * Return values
 * -1: Must not cool, current temperature is below minimum
 * 0: May cool if others require so, temperature is within range
 * 1: Need to cool, current temperature is above maximum
*/
int controller(float current_temperature, float minimum_temperature, float maximum_temperature) {
  if (current_temperature >= maximum_temperature)
    return 1;

  if (current_temperature <= minimum_temperature)
    return -1;

  return 0;
}

void controller_loop(float temperature_fridge, float temperature_freezer) { //Called from loop to check if the state has to be changed
  int result_fridge=0, result_freezer=0;

  #if HAS_FRIDGE==true
    if (fridge_is_on())
      result_fridge = controller(temperature_fridge, fridge_get_minimum_temperature(), fridge_get_maximum_temperature());
    else
      result_fridge = -2;
  #else
    result_fridge = -2;
  #endif

  #if HAS_FREEZER==true
    if (freezer_is_on())
      result_freezer = controller(temperature_freezer, freezer_get_minimum_temperature(), freezer_get_maximum_temperature());
    else
       result_freezer = -2;
  #else
    result_freezer = -2;
  #endif

  if (result_fridge == -2 && result_freezer == -2) { //Stop cooling if fridge & compressor have been turned off
    compressor_off();
  } else if (result_fridge == 1 || result_freezer == 1) { //Start cooling
    compressor_on();
  } else if (result_fridge == -1 || result_freezer == -1) { //Stop cooling
    compressor_off();

    if (preset_boost_active()) { //Disable boost mode if enabled
      preset_mode_none();
    }
  }
}

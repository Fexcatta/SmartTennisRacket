# ArduinoBLE

Modified version of ArduinoBLE library.

The change (made in https://github.com/arduino-libraries/ArduinoBLE/commit/a74270d072e056a7c68a5445ba00e1e4b78ec151) has been reverted. The timer is no more started.

In the HCICordioTransport.cpp(line:114) file:
```c++
//timer.start();
```

# Arduino_BMI270_BMM150

Modified version of Arduino_BMI270_BMM150 library.

Changelog:

- I2C clock speed (400 KHz)
- Accelerometer and gyroscope configuration
  - OutputDataRate to 800Hz
  - Accelerometer Full Scale Range to ±16 g

I2C clock speed: BMI270.ccp(line:40) 
```c++
  _wire->setClock(400000);
```

Accelometer ODR: BMI270.ccp(line:220)
```c++
  sens_cfg[0].cfg.acc.odr = BMI2_ACC_ODR_800HZ;
```

Accelometer FSR: BMI270.ccp(line:222)
```c++
  sens_cfg[0].cfg.acc.range = BMI2_ACC_RANGE_16G;
```

Gyroscope ODR: BMI270.ccp(line:226)
```c++
  sens_cfg[1].cfg.gyr.odr = BMI2_GYR_ODR_800HZ;
```

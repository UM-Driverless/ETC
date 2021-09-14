# NORMATIVA ETC

## T 11.8 Accelerator Pedal Position Sensor (APPS)

T 11.8.2 The APPS must be actuated by a foot pedal.
T 11.8.3 Pedal travel is defined as percentage of travel where 0 % is fully released and 100 % is fully applied.
T 11.8.4 The foot pedal must return to the 0 % position when not actuated. The foot pedal must have
a positive stop preventing the mounted sensors from being damaged or overstressed. Two
springs must be used to return the foot pedal to the 0 % position and each spring must work
when the other is disconnected. Springs in the APPS are not accepted as return springs.

T 11.8.5 At least **two separate sensors** must be used as APPSs. Separate is defined as not sharing
supply or signal lines.
T 11.8.6 If analog sensors are used, they must have different, **non-intersecting transfer functions**, . A
short circuit between the signal lines must always result in an implausibility according to T 11.8.9.
T 11.8.7 The APPS **signals are SCSs**, see T 11.9.
T 11.8.8 If an **implausibility occurs between the values of the APPSs for more than 100 ms**:
 - [EV ONLY] The power to the motor(s) must be immediately shut down completely. It
is not necessary to completely deactivate the tractive system, the motor controller(s)
shutting down the power to the motor(s) is sufficient.
 - [CV ONLY] The **power to the electronic throttle must be immediately shut down**.
T 11.8.9 Implausibility is defined as a deviation of more than ten percentage points pedal travel
between any of the used APPSs or any failure according to T 11.9.
T 11.8.10 If three sensors are used, then in the case of an APPS implausibility, any two sensors that are
plausible may be used to define the torque target and the 3rd APPS may be ignored.
T 11.8.11 It must be possible to **separately disconnect each APPS signal wire to check all functionalities**.
T 11.8.12 A fully released accelerator pedal must result in:
• [EV ONLY] A wheel torque of ≤0 N m
• [CV ONLY] **An idle position or lower throttle set-point. This may only be exceeded during a gearshift for a maximum of 500 ms.**


## CV 1.6 Electronic Throttle Control (ETC)
CV 1.6.2 [DV ONLY] Any DV with internal combustion engine is assumed to have ETC.
CV 1.6.3 The team must be able to demonstrate the functionality of all safety features and error
detections of the ETC system at technical inspection, see IN.

CV 1.6.4 The ETC system must be equipped with at least the following sensors:
 - Accelerator Pedal Position Sensors (APPSs) as defined in T 11.8.
 - Two Throttle Position Sensors (TPSs) to measure the throttle position.
 
CV 1.6.5 All ETC signals are **SYSTEM CRITICAL SIGNALS (SCSs)**, see T 11.9.

CV 1.6.6 **When power is removed, the ET must immediately close at least to idle position ±5, in an interval of one second**. 
**Failure to achieve this must result in immediate disabling of power to ignition, fuel injectors and fuel pump**. 
**This action must remain active until the TPS signals indicate the throttle has returned to idle position ±5 % for at least one second**.

CV 1.6.7 **If implausibility between TPSs for more than 100 ms, the power to the electronic throttle must be immediately shut down**.
Plausibility is defined as a deviation of less than ten percentage points between the sensor
values as defined in CV 1.4.3 and no detected failures as defined in T 11.9.

[DV ONLY] Autonomous system must check this signal consistency on a low level itself.

CV 1.6.8 The electronic throttle must use at least two sources of energy capable of returning the
throttle to the closed position. One of the sources may be the device that normally actuates
the throttle, e.g. a DC motor, but the other device(s) must be a return spring that can return
the throttle to the idle position in the event of a loss of actuator power.
CV 1.6.9 Springs in the TPSs are not acceptable as **return springs**.

CV 1.6.10 **The power to the ET must be immediately shut down, as defined in CV 1.6.6**,
**if the TP differs by more than 10 % from the expected target TPS position for more than 500 ms**.


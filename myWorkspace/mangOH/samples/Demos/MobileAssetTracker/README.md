MobileAssetTracker
================

This app provides a way to gather sensor and location data from the built-in sensors of a
mangOH Red and publish that data to AirVantage using LWM2M.  This app
attempts to conserve bandwidth by publishing only when the sensor readings
exceed preprogrammed thresholds or if no readings have been published for quite
a while.

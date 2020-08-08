# RateLimiter
Arduino library for rate limiting function calls. This library implements the [token bucket](https://en.wikipedia.org/wiki/Token_bucket) algorithm.
Calls can be either dropped or blocked (delayed to satisfy rate limit) if they are made too fast.
Unlike simpler rate limiting algorithms, the limited calls will be evenly spaced out in the case of calls coming too fast. 

## Examples:
* Limit logging rate (for example, because you are logging over the network)
* Periodically sync time with an NTP server
* Limit the frame rate of a rendering function or simulation
* Limit calls per second to an external API
* Limit query per second from an user to prevent DoS attack

## Usage:
    // Create a limiter limiting call rate to 5 per 1000ms.
    static RateLimiter<1000, 5> limiter;

    // Call a function only if it would be within rate limit.
    limiter.CallOrDrop([&]() { do something });

    // Call a function but potentially with a delay inserted to
    // make sure we don't go over the rate limit.
    limiter.Call([&]() { do something });

    // Register a function to be called if there are dropped calls.
    limiter.SetDroppedCallCallback([](unsigned int dropped_calls) {
      Serial.println(String("... dropped ") + dropped_calls + " calls...");
    });
See examples/RateLimiterTest for the interactive testsuite and more detailed examples. In most cases
the easiest way to use the library would be using [lambdas](https://en.cppreference.com/w/cpp/language/lambda) (as shown in the examples), but you can
also call regular functions with it.

## Installation:
1. Download [the zip file](https://github.com/matthewlai/RateLimiter/archive/master.zip)
2. Sketch -> Include Library -> Add ZIP Library -> Select the downloaded file
3. Sketch -> Include Library -> RateLimiter

## License:
This library is licensed under the MIT License. You may use it in any way you want (including
modifying it or using it in commercial projects), but the author(s) is(are) not providing any warranty
or assuming liability. See the LICENSE file for more details.

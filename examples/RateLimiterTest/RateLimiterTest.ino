// MIT License
//
// Copyright (c) 2020 Matthew Lai
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <RateLimiter.h>

// Example output:
//  Example 1: some calls dropped
//  Time: 1052
//  Time: 1052
//  Time: 1052
//  Time: 1052
//  Time: 1052
//  This should work
//  Example 2: no calls dropped, but calls slowed down for rate limiting
//  Time: 1302
//  Time: 1302
//  Time: 1302
//  Time: 1302
//  Time: 1302
//  Time: 1502
//  Time: 1702
//  Time: 1902
//  Time: 2102
//  Time: 2302
//  Example 3: log how many calls we are dropping
//  Time: 2302
//  Time: 2302
//  Time: 2302
//  Time: 2302
//  Time: 2302
//  ... dropped 5 calls...
//  We should see that 5 calls have been dropped before this message
//  Example 4: with a normal function instead of lambda
//  print_message()
//  print_message()
//  print_message()

void example1() {
  Serial.println("Example 1: some calls dropped");

  // Limit call rate to 5 per 1000ms.
  static RateLimiter<1000, 5> limiter;

  // Quickly make 5 calls. They should all pass.
  for (int i = 0; i < 5; ++i) {
    limiter.CallOrDrop([&]() {
      Serial.println(String("Time: ") + millis());
    });
  }

  // Another one should be dropped.
  limiter.CallOrDrop([&]() {
    Serial.println("You shouldn't see this");
  });

  // Wait a bit.
  delay(250);

  // Now this one should pass through again.
  limiter.CallOrDrop([&]() {
    Serial.println("This should work");
  });
}

void example2() {
  Serial.println("Example 2: no calls dropped, but calls slowed down for rate limiting");

  // Limit call rate to 5 per 1000ms.
  static RateLimiter<1000, 5> limiter;

  // Quickly make 10 calls. They should all pass, but the last 5 should be delayed.
  // No call should be dropped because we are using Call instead of CallOrDrop.
  for (int i = 0; i < 10; ++i) {
    limiter.Call([&]() {
      Serial.println(String("Time: ") + millis());
    });
  }
}

void example3() {
  Serial.println("Example 3: log how many calls we are dropping");
  
  static RateLimiter<1000, 5> limiter;
  limiter.SetDroppedCallCallback([](unsigned int dropped_calls) {
    Serial.println(String("... dropped ") + dropped_calls + " calls...");
  });

  for (int i = 0; i < 10; ++i) {
    limiter.CallOrDrop([&]() {
      Serial.println(String("Time: ") + millis());
    });
  }

  delay(250);
  limiter.CallOrDrop([&]() {
    Serial.println("We should see that 5 calls have been dropped before this message");
  });
}

void print_message() {
  Serial.println("print_message()");
}

void example4() {
  Serial.println("Example 4: with a normal function instead of lambda");

  // Limit call rate to 5 per 1000ms.
  static RateLimiter<1000, 5> limiter;

  for (int i = 0; i < 3; ++i) {
    limiter.Call(print_message);
  }
}

void print_number(int x) {
  Serial.println(String("print_number(") + x + ")");
}

void example5() {
  Serial.println("Example 5: use lambda to pass argument into normal function");

  // Limit call rate to 5 per 1000ms.
  static RateLimiter<1000, 5> limiter;

  for (int i = 0; i < 3; ++i) {
    limiter.Call([&]() { print_number(i); });
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  example1();
  example2();
  example3();
  example4();
}

void loop() {}
#include "qt_time_provider.h"

s21::QtTimeProvider::QtTimeProvider() { timer_.start(); }

int s21::QtTimeProvider::GetTimeInMilliseconds() { return timer_.elapsed(); }

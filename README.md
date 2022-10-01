# atom

![license](https://img.shields.io/github/license/fleroviux/atom)

atom is a collection of C++ libraries that I have written for use in my personal projects.

## Motivation

I used to have the same or similar code for things like logging, linear algebra and error handling in multiple projects.
This was not ideal, as improvements made to the code in one project, could not be quickly and easily brought over to the other projects.

atom is an attempt to maintain my core libraries in a single place and with a consistent code style to maximize code reuse and minimize maintenance effort.

## Modules

- Atom Common:
  - Sized integer types (u8, s8, u16, s16, ...)
  - Panic macro with support for a custom panic handler

- Atom Logger:
  - Logger with multi-sink support
  - Per-logger and per-sink log levels
  - Support for custom sinks
  - Support for sharing sink collections between loggers
  
- Atom Math:
  - Vector2, Vector3, Vector4
  - Matrix4
  - Quaternion
  - Box3 (axis-aligned bounding box)
  - Frustum
  - Plane

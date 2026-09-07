# Game Programming Patterns Lab

An Unreal Engine C++ learning project for implementing and evaluating the
patterns from Robert Nystrom's
[Game Programming Patterns](https://gameprogrammingpatterns.com/).

The goal is not merely to reproduce each pattern, but to:

- Begin with a simple or deliberately naive implementation
- Expose the problem the pattern is intended to solve
- Apply the pattern in an Unreal-appropriate way
- Compare the resulting benefits and trade-offs
- Profile optimization patterns before drawing conclusions

## Technology

- Unreal Engine 5.8
- C++
- Blueprints for asset configuration and visual presentation
- Enhanced Input

## Project Philosophy

Patterns are treated as design trade-offs rather than rules. A pattern is introduced only after demonstrating the concrete problem it addresses.

Optimization patterns will be supported by measurements captured with Unreal Insights or other appropriate profiling tools.

## Building

1. Install Unreal Engine 5.8.
2. Clone the repository using Git LFS.
3. Right-click `GamePatternsLab.uproject` and generate project files if needed.
4. Build the `GamePatternsLabEditor` target.
5. Open `GamePatternsLab.uproject`.

## Licensing

Original source code in this repository is licensed under the MIT License.

Unreal Engine and Epic-provided assets remain subject to their respective Epic Games license agreements and are not relicensed by this repository.

This is an independent educational project inspired by Robert Nystrom's *Game Programming Patterns*. It is not affiliated with Robert Nystrom or Epic Games.

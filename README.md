# Neofluxon

Here I am trying to develop a simple photography workflow software, first for myself,
as a tool that fits my needs and is not intended for advanced users.

A little philosophy behind this may change or improve over time; these are the central ideas:

- Filesystem is the source of truth. The main organization is the filesystem.
- Files outlive software. Photos, edits, and exports should remain accessible independently of the application.
- Workflow defines organization. Sessions group originals, edits, and exports as they are produced.
- Non-destructive. Originals are never modified.
- Data assets, organization, and edits remain accessible. They stay usable with standard tools, without dependency on the software.

Every import is defined as a Session/Shoot.

A shoot/session will generally represent a standard linear workflow:
import → culling → edit → export → archive

According to the third law of thermodynamics, entropy always increases.
No matter how organized a user might be, chaos eventually wins.
The plan is for the software to enforce/automate a filesystem structure for data assets along the workflow steps.

The first stage focuses on a single part of the workflow: organization of digital assets:
import → culling → archive

The second stage will be non-destructive editing and export.

Currently, there is no release yet; the code is in an experimental/development stage.
You can check the repository tags. At the moment, it can browse the file system and display thumbnails for CR3 RAW files.

It is developed in C++ and Qt. The code is split into two parts:
the core (which is pure C++ and not dependent on Qt), and the UI part written in Qt (C++).

Neofluxon is free software (GPLv3).

## Dependencies

- Qt6
- LibRaw (https://www.libraw.org)

### Build

- CMake ≥ 3.25
- C++23

# SNHU-CS300
This repository contains two core artifacts showcasing data structures, sorting algorithms, and performance analysis:
Project One: Performance analysis of run-time and memory complexities for various data structures.
Project Two: Working C++ implementation that parses, sorts, and prints out a list of Computer Science program courses in alphanumeric order.

What was the problem you were solving in the projects for this course?
The core problem addressed across these projects involved efficiently storing, organizing, retrieving, and sorting structured data. In Project One, the focus centered on evaluating how different data structures, such as vectors, hash tables, and binary search trees, handle data storage and retrieval, and analyzing their computational efficiency in terms of time and memory. In Project Two, the objective shifted to implementing a functional data processing system capable of loading a dataset containing course identifiers and titles, sorting them cleanly in alphanumeric order, and handling user navigation through the curriculum data reliably.

How did you approach the problem? Consider why data structures are important to understand.
My approach began by analyzing the operational requirements of the dataset, specifically examining whether the application required frequent searches, insertions, deletions, or sorted traversal. Understanding data structures is critical because choosing the right structure directly dictates program scalability and resource utilization. For instance, utilizing a hash table allows for rapid key-value lookups, whereas a binary search tree naturally maintains ordered data elements, and vectors provide efficient sequential access. Recognizing these underlying mechanics enables software engineers to design solutions that perform reliably even as data scales.

How did you overcome any roadblocks you encountered while going through the activities or project?
Roadblocks typically manifested as memory management challenges, pointer handling bugs, or unexpected sorting behaviors when parsing CSV files. I overcame these hurdles by systematically debugging using structured IDE tools, isolating specific functions, and reviewing algorithmic logic step-by-step. Validating edge cases, such as empty files or malformed input records, ensured robust error handling and stable execution.

How has your work on this project expanded your approach to designing software and developing programs?
Working through these data structure and algorithm implementations has broadened my perspective beyond merely writing code that works functionally to writing code that performs efficiently under constraints. I now place greater emphasis on algorithmic complexity during the initial design phase, evaluating Big-O implications before committing to a specific container or search strategy. This analytical mindset ensures that software architectures remain lean, responsive, and optimized for performance.

How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?
This coursework reinforced the necessity of clean code standards, modular design, and descriptive variable naming. By separating data loading logic, node management, and user interface routines into distinct classes and methods, the resulting codebase becomes significantly easier to maintain, test, and extend. Adaptability improves when components are decoupled, allowing future developers to swap underlying data structures, such as transitioning from a vector to a hash map, without rewriting entire application layers.

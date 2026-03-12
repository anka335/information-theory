# Information Theory and NLP Algorithms

This repository contains a collection of implementations focused on information theory and lossless data compression. The project is divided into four main assignments.


## Assignments 

### 1. Markov Source Approximations


This module generates text approximations of the English language using statistical probability models.
* Implements 1st-order, 3rd-order, and 5th-order Markov sources.
* Uses lower-order models to seed the initial characters of higher-order generations.
* Calculates the average word length for the generated text approximations.
* Features memory optimizations, utilizing frequency thresholds ($k$) to drop rare character sequences.

### 2. Natural Language Detection
This module calculates text predictability to distinguish real human languages from randomly generated text.
* Computes standard character and word entropy, alongside conditional entropies of higher ranks (measured in bits).
* Establishes baseline entropy metrics using normalized English and Latin corpuses.
* Evaluates six unknown text samples (`sample0.txt` to `sample5.txt`) to determine and justify whether they are written in a natural language.

### 3. Huffman Coding


A custom implementation of David Huffman's 1951 algorithm for efficient binary prefix coding.
* Built from scratch utilizing a priority queue.
* Includes core functions: `create` (builds the dictionary), `encode`, and `decode`.
* Outputs the specific binary codes assigned to characters.
* Compresses text corpuses and compares the final compressed bit size against the shortest possible fixed-length encoding.

### 4. LZW Compression


An implementation of the Lempel-Ziv-Welch (LZW) dictionary-based compression method, originally published in 1984.
* Dynamically builds the translation dictionary on the fly during both compression and decompression.
* Compresses standard text files, handling extended codes beyond standard ASCII.
* Processes and compresses binary data, specifically tested on image files like `fuji_02.png`.

---

## Data Sources
The text corpuses and samples used for training and testing these algorithms include normalized text (lowercase Latin letters, digits, spaces) for languages such as English, Latin, Esperanto, Estonian, Somali, Haitian, and Navaho. 

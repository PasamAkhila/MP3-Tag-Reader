# MP3 Tag Reader and Editor

## Project Description

The MP3 Tag Reader and Editor is a C-based application designed to read, display and modify metadata information stored in MP3 audio files.

It supports ID3v2.3 metadata format and allows users to view and edit information such as Title, Artist, Album, Year and Composer.

The project demonstrates practical implementation of file handling, structures, pointers and dynamic memory allocation in C.

## Features

- Read and display MP3 metadata.
- Edit existing MP3 tag information.
- Supports ID3v2.3 format.
- Displays Title, Artist, Album, Year and Composer.
- Validates MP3 file format and ID3 header.
- Uses file handling to read and modify metadata.
- Supports command-line arguments.
- Provides help instructions for users.

## Technologies Used

- Programming Language: C
- Metadata Format: ID3v2.3
- Concepts: File Handling, Structures, Pointers, Dynamic Memory Allocation, Bitwise Operations and Command-Line Arguments.

## Working Principle

### 1. Tag Reading

1. Accept the MP3 file through command-line arguments.
2. Open the file in binary read mode.
3. Validate the ID3 header.
4. Extract the ID3 tag size.
5. Read individual metadata frames.
6. Extract and display the corresponding tag information.

### 2. Tag Editing

1. Accept the MP3 file and the tag to be modified.
2. Validate the command-line arguments.
3. Open the original MP3 file.
4. Validate the ID3v2.3 header.
5. Locate the required metadata frame.
6. Update the selected tag information.
7. Save the modified MP3 file.

## Metadata Supported

| Tag | Description |
|---|---|
| TIT2 | Title |
| TPE1 | Artist |
| TALB | Album |
| TYER | Year |
| TCOM | Composer |

## Concepts Implemented

- ID3v2.3 metadata parsing.
- Binary file handling.
- Frame identification and extraction.
- Dynamic memory allocation.
- Structures and pointers.
- Command-line argument validation.
- File validation and error handling.

## Learning Outcomes

- Gained practical experience in binary file handling.
- Understood the ID3v2.3 metadata structure.
- Learned to read and modify metadata frames.
- Improved understanding of pointers and dynamic memory allocation.
- Developed skills in command-line argument handling.
- Improved debugging and problem-solving skills.

## Author

**Pasam Akhila**

B.Tech – Electronics and Communication Engineering

Steganography Using C

Steganography project written in C which allows you to hide and get your secret messages into images files. The example showcases the principle of data security way by injecting text data to the pixels in a picture without altering the original image in visible aspects.


Overview

Steganography is hiding information by embedding it in another medium, so that the user is unaware of any such information. In this project, various LSB encoding methods were used to hide and extract secret messages from bitmap images.
This is a hobby project created for the purpose of study into image file structures, bit-wise data manipulation & secure data embedding.

Features

Embed hidden text messages within image files
Decipher secret messages from encoded images
Greatly perpeptuate the visual resemblance of original image
Then some bitwise manipulation we do to encode data
Encode and decode command line functions
If the file we gave doesn't exist or if our inputs are invalid, we show a proper error message to help resolve it in both cases.


Technologies Used

C Programming
File Handling
Bitwise Operations
Image Processing Concepts
Data Encoding and Decoding Techniques


Working Principle

Encoding Process
Read the source image file.
Extract the secret message from a text file.
Represent message characters as binary format
In the first step, we embed the binary data into the LSBs of image pixels.
Create a new encoded image that contains the hidden message.
Decoding Process
Read the encoded image.
Extract data hidden in the Least Significant Bits.
Reconstruct the binary information.
Decode text back into binary data.
Release or store the identified key in new message.

Project Structure

├── encode. c ├── decode. c ├── common. h ├── encode. h ├── decode. h ├── test_image. bmp ├── secret. txt ├── output. bmp └── README. md

Learning Outcomes

This project allowed me the opportunity to work with:

Familiarity with image file formats and headers
File input/output operations in C
Bitwise data manipulation
Data hiding techniques
Debugging and memory management
Information security fundamentals

Challenges Faced

A big hurdle was figuring out how to run bitwise operations correctly for both of encoding and decoding processes. Embedding data while keeping the image intact involved tinkering with pixel values. A lot of testing, debugging and self-learning were employed to tackle these hurdles.
Future Improvements
Support for multiple image formats
Password-protected message extraction
Encryption before embedding
Graphical User Interface (GUI)
Support for larger file embedding


Author

Varsha Natashekara

MSc Sensor Systems Technology

Hochschule Karlsruhe, Germany

LinkedIn: www.linkedin.com/in/varsha-n-b42593246

License

This project is for educational and learning purposes only.

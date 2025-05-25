# 📷 Image Processing in C – TI202I Project

## 👥 Authors
- **DEMOULIN Hugo** – Hug0dml
- **UNMAR Emeline** – LeyaUnm

---

## 🗂 Project Structure

├── bmp8.h / bmp8.c # Functions for 8-bit grayscale images

├── bmp24.h / bmp24.c # Functions for 24-bit color images

├── histogram_equalization.c # Histogram equalization (grayscale and color)

├── main.c # Main menu and user interface

├── images/ # Folder for .bmp test images

└── README.md # This file

---

## 📌 Project Objectives

This project aims to build an image processing tool in the C programming language. It is split into 3 main parts:

1. **8-bit Grayscale Image Processing**
   - Read and write BMP files
   - Negative, brightness, and threshold operations
   - Apply convolution filters (box blur, sharpen, outline, etc.)

2. **24-bit Color Image Processing**
   - Manipulate RGB pixel data
   - Convert to grayscale
   - Apply color convolution filters

3. **Histogram Equalization**
   - Improve contrast by redistributing gray levels
   - Equalize both grayscale and color images (via YUV space)

---

## 🖥️ How to Compile

```bash
gcc main.c bmp8.c bmp24.c histogram_equalization.c filters.c -o image_processor -lm
```

##▶️ How to Run

```bash
./image_processor
```
Then follow the menu options to:

-Open an image

-Apply filters

-Equalize histogram

-Save the result

##🧪 Test Images

Place your test .bmp files in the images/ directory.
Recommended test files:

barbara_gray.bmp (grayscale)

flowers_color.bmp (color)

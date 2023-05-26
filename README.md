# Decomposing into Convex Polygons

This program implements the algorithm specified in the research paper titled [Algorithms for the decomposition of a polygon into convex polygons](/ResearchPaper.pdf).  
The program is a part of submission for the assignment of **CS F364 - Design And Analysis of Algorithms** course of **[BITS Pilani, Hyderabad Campus](https://www.bits-pilani.ac.in/hyderabad/)**.

## Running the program

1. Place the input in 'input.txt' file in the format:  

```text
N
x1 y1
x2 y2
.
.
.
xn yn
```  

Where N is the number of points and the coordinates are to be entered in clockwise order.

2. Compile the 'decompose.cpp file': `gcc decompose.cpp -o decompose.out` or `gcc decompose.cpp -o decompose.exe`.
3. Run the compiled executable: `./decompose.out` or `./decompose.exe`.
4. The output is generated in the 'output,txt' file.

## File Structure

- **DOXYGEN**: This folder contains `index.html` file that contains the code documentation.
- **Jupyter Notebooks**: This folder contains .ipynb files to generate points, visualise decomposition of polygons at various stages, etc.
- **REPORT HTML**: This folder contains `Report.html` that contains the implementational documentation of the program.

## Group Members

- **[Ritvik](https://github.com/Code-R57)**
- **[Abhinav Tyagi](https://github.com/Abhiinv)**
- **[Dhruv Merchant](https://github.com/DhruvMerchant)**
- **[Uday Singh Thakur](https://github.com/udaysingh50)**

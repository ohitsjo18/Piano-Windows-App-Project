# Piano-Windows-App-Project

## Project Description 

This project contains the C++ program code and source files required to run a basic GUI Piano windows application in Windows 10. 

The code currently spins up a Windows app with a single-octave piano graphic, which plays the appropriate sounds when a key is clicked on. 

The purpose of this project is simply to be a fun C++ beginner/intermediate exercise: it demonstrates some basic C++ fundamentals  as well as provides a simplistic 
introduction to creating C++ windows applications and using graphics libraries. 

## Running the Project 

### Prerequisites 
- Windows 10 
- Visual Studio Code installed with <Desktop Development for C++> workload (See: Prerequisites - https://docs.microsoft.com/en-us/cpp/windows/walkthrough-creating-windows-desktop-applications-cpp?view=vs-2019) 

### Creating a Windows Desktop Project 

These instruction are the same as the official Microsoft Docs, pasted here for convenience: 
https://docs.microsoft.com/en-us/cpp/windows/walkthrough-creating-windows-desktop-applications-cpp?view=vs-2019

1. Open your Visual Studio Code Application 
2. Create a new project: Select File > New > Project 
3. Choose the <Windows Desktop Wizard> project type > Click Next 
4. Enter your project name and create the project 
5. Under application type, select <Deskptop Application (.exe)>. Select <Empty Project>. Select OK to create the project. 
6. This should open up your new, empty project. In the solution explorer, go to <Source Files> and add PianoApp.cpp as an existing item. 
7. Add stdafx.h as an empty header file to the project: Go to solution explorer, Source Files > Add New Item > Add header file called stdafx.h 
8. Download the Piano Sound files provided and save them in a convenient location in your local system. Update PianoApp.cpp with the filepaths to your local sound files. 
8i. Please note - sound files must be of .wav format (so they are compatible with PlaySound() function). Ensure \ characters in filepaths are escaped (e.g. "C:\\Directory1\\Sound_Files\\sound.wav"). 

## How to collaborate 

Sorry, but this project is currently not open for collaboration, as it serves as a personal practice project. The project is public for resume/hiring purposes. 
However, I am a beginner C++ programmer so am very open to advice, suggestions and improvements! I can be reached at jonock18@gmail.com

## About Me 

✿ Joanne Nock ✿

MEng Computer Systems Engineering student at the University of Warwick (third year) 

Passionate about health-tech, enviro-tech and making the world a better place one line of code at a time! 

LinkedIn Profile: https://www.linkedin.com/in/joannenock/


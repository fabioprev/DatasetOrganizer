DatasetOrganizer
=========

# Requirements

DatasetOrganizer requires the following packages to build:
  
  * build-essential
  * g++
  * cmake
  * libxml2

On Xubuntu (Ubuntu) 14.04.4 LTS (kernel 4.2.0-27), these dependencies are
resolved by installing the following packages:
  
  - build-essential
  - cmake
  - libxml2
  - libxml2-dev

# How to build

The only development platform is Linux. We recommend a so-called out of source
build which can be achieved by the following command sequence:
  
  - mkdir build
  - cd build
  - cmake ../src
  - make -j\<number-of-cores+1\>

# Usage

In order to execute DatasetOrganizer you just need to type in to a terminal the following command:

  - cd bin
  - ./DatasetOrganizer -d \<directory-root\>

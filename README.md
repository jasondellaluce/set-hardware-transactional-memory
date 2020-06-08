# Set - Hardware Transactional Memory
Research project for graduate class CS295 - Advanced Concurrency (Spring 2020, University of California - Irvine)

## Introduction
This experiment explores new concurrency approaches based on transactional memory. Most recent CPUs (e.g. Intel starting from Haswell) provided support for Hardware Transactional Memory (HTM). The concept proposes the usage of transactions as an alternative to classic locking approaches to solve concurrency problems. Leveraging hardware support, transactions can provide a significant performance improvement in the general case, allowing multiple threads to speculative execute in parallel and handle data conflicts with abortion.

## Contents
This repository contains various implementations of a set data structure. The goal is to measure the performance of various concurrent implementations, and compare them to identify to which extent HTM can be beneficial. A set is a collection that doesn't allow multiple instances of the same element to be present. This implementation exposes the most basic methods:
- **add**: Inserts an element in the collection, if not already present
- **remove**: Removes an element from the collection, if already present
- **contains**: Asserts whether an element is present in the collection or not

The data structure have been implemented as an ordered linked list. Multiple concurrency solutions have been developed for comparison:
- Coarse-grain locking, with a unique shared lock for each operation of the collection
- Lock-free with lazy removing strategy based on markable atomic variables
- Hybrid Transactional Memory using g++ proposed standard libITM
- Hardware Transactional Memory using Intel's RTM low-level operators

Each HTM approach have been implemented in with coarse granularity, namely creating a transaction for each entire operation of the data structure. This has been done in order to easily compare their performance with classical coarse-grain locking.


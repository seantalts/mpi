// Copyright (C) 2006 Douglas Gregor <doug.gregor -at- gmail.com>

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file config.hpp
 *
 *  This header provides MPI configuration details that expose the
 *  capabilities of the underlying MPI implementation, and provides
 *  auto-linking support on Windows.
 */
#ifndef BOOST_MPI_CONFIG_HPP
#define BOOST_MPI_CONFIG_HPP

/* Force MPICH not to define SEEK_SET, SEEK_CUR, and SEEK_END, which
   conflict with the versions in <stdio.h> and <cstdio>. */
#define MPICH_IGNORE_CXX_SEEK 1

#ifndef BOOST_MPI_SEQ
#include <mpi.h>
#else
#include <boost/mpi/seq.hpp>
#endif
#include <boost/config.hpp>

/** @brief Comment this macro is you are running in an heterogeneous environment.
 *
 * When this flag is enabled, we assume some simple, POD-like, type can be 
 * transmitted without paying the cost of portable serialization. 
 *
 * Comment this if your platform is not homogeneous and that portable 
 * serialization/deserialization must be performed.
 *
 * It you do so, check that your MPI implementation supports thats kind of environment.
 */
#define BOOST_MPI_HOMOGENEOUS

// If this is an MPI-2 implementation, define configuration macros for
// the features we are interested in.
#if defined(MPI_VERSION) && MPI_VERSION >= 2
/** @brief Determine if the MPI implementation has support for memory
 *  allocation.
 *
 *  This macro will be defined when the underlying MPI implementation
 *  has support for the MPI-2 memory allocation routines @c
 *  MPI_Alloc_mem and @c MPI_Free_mem. When defined, the @c allocator
 *  class template will provide Standard Library-compliant access to
 *  these memory-allocation routines.
 */
#  define BOOST_MPI_HAS_MEMORY_ALLOCATION

/** @brief Determine if the MPI implementation has supports initialization 
 *  without command-line arguments.
 *
 *  This macro will be defined when the underlying implementation
 *  supports initialization of MPI without passing along command-line
 *  arguments, e.g., @c MPI_Init(NULL, NULL). When defined, the @c
 *  environment class will provide a default constructor. This macro is 
 *  always defined for MPI-2 implementations. */
#  define BOOST_MPI_HAS_NOARG_INITIALIZATION
#else
// If this is an MPI-1.x implementation, no arg initialization for
// mpi environment could still be available, but not mandatory.
// Undef this if no arg init is available:
//#  define BOOST_MPI_HAS_NOARG_INITIALIZATION
#endif

#if defined(MPIAPI)
#  define BOOST_MPI_CALLING_CONVENTION MPIAPI
#else
/** @brief Specifies the calling convention that will be used for callbacks
 *  from the underlying C MPI. 
 *
 *  This is a Windows-specific macro, which will be used internally to state
 *  the calling convention of any function that is to be used as a callback
 *  from MPI. For example, the internally-defined functions that are used in
 *  a call to @c MPI_Op_create. This macro is likely only to be useful to
 *  users that wish to bypass Boost.MPI, registering their own callbacks in
 *  certain cases, e.g., through @c MPI_Op_create.
 */
#  define BOOST_MPI_CALLING_CONVENTION
#endif

/** @brief Indicates that MPI_Bcast supports MPI_BOTTOM.
 *
 * Some implementations have a broken MPI_Bcast wrt to MPI_BOTTOM.
 * BullX MPI and LAM seems to be among them, at least for some versions.
 * The `broacast_test.cpp` test `test_skeleton_and_content` can be used to 
 * detect that.
 */
#define BOOST_MPI_BCAST_BOTTOM_WORKS_FINE

#if defined(LAM_MPI)
// Configuration for LAM/MPI
#  define BOOST_MPI_HAS_MEMORY_ALLOCATION
#  define BOOST_MPI_HAS_NOARG_INITIALIZATION
#  undef  BOOST_MPI_BCAST_BOTTOM_WORKS_FINE
#elif defined(MPICH_NAME)
// Configuration for MPICH
#endif

/*****************************************************************************
 *                                                                           *
 *  DLL import/export options                                                *  
 *                                                                           *
 *****************************************************************************/

#if (defined(BOOST_MPI_DYN_LINK) || defined(BOOST_ALL_DYN_LINK)) && !defined(BOOST_MPI_STATIC_LINK)
#  if defined(BOOST_MPI_SOURCE)
#     define BOOST_MPI_DECL BOOST_SYMBOL_EXPORT
#     define BOOST_MPI_BUILD_DLL
#  else
#     define BOOST_MPI_DECL BOOST_SYMBOL_IMPORT
#  endif
#endif

#ifndef BOOST_MPI_DECL
#  define BOOST_MPI_DECL
#endif

#if !defined(BOOST_MPI_NO_LIB) && !defined(BOOST_MPI_SOURCE) && !defined(BOOST_ALL_NO_LIB) && defined(__cplusplus)
#  define BOOST_LIB_NAME boost_mpi
#  if defined(BOOST_MPI_DYN_LINK) || defined(BOOST_ALL_DYN_LINK)
#     define BOOST_DYN_LINK
#  endif
#  ifdef BOOST_MPI_DIAG
#     define BOOST_LIB_DIAGNOSTIC
#  endif
#  include <boost/config/auto_link.hpp>
#endif

#endif // BOOST_MPI_CONFIG_HPP

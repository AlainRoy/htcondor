#######################################################################
# CHECK_PROG_IS_GNU is based loosely on the CHECK_GNU_MAKE macro 
# available from the GNU Autoconf Macro Archive at:
# http://www.gnu.org/software/ac-archive/htmldoc/check_gnu_make.html
# This version is more generic in that it checks any given program if
# it's GNU or not.  Written by Derek Wright <wright@cs.wisc.edu>
# Arguments:
#  $1 is the PATH of the program to test for
#  $2 the variable name you want to use to see if it's GNU or not
# Side effects:
#  Sets _cv_<progname>_is_gnu to "yes" or "no" as appropriate
#######################################################################
AC_DEFUN( [CHECK_PROG_IS_GNU],
[AC_CACHE_CHECK( [if $1 is GNU], [_cv_$2_is_gnu],
 [if ( sh -c "$1 --version" 2> /dev/null | grep GNU > /dev/null 2>&1 ) ;
  then
     _cv_$2_is_gnu=yes
  else
     _cv_$2_is_gnu=no
  fi
 ])
])


AC_DEFUN( [CHECK_TAR_OPTION],
 [AC_CACHE_CHECK( [if tar supports $1], [$2],
  [( $_cv_gnu_tar -cf _cv_test_tar.tar $1 configure > /dev/null 2>&1 )
  _tar_status=$?
  rm -f _cv_test_tar.tar 2>&1 > /dev/null
  if test $_tar_status -ne 0; then
    $2="no";
  else
    $2="yes";
  fi
 ])
])


dnl Available from the GNU Autoconf Macro Archive at:
dnl http://www.gnu.org/software/ac-archive/htmldoc/ac_prog_perl_version.html
dnl
AC_DEFUN([AC_PROG_PERL_VERSION],[dnl
# Make sure we have perl
if test -z "$PERL"; then
  AC_PATH_PROG(PERL,perl,no,[$PATH])
fi
# Check if version of Perl is sufficient
ac_perl_version="$1"
if test "$PERL" != "no"; then
  AC_MSG_CHECKING(for perl version greater than or equal to $ac_perl_version)
  # NB: It would be nice to log the error if there is one, but we cannot rely
  # on autoconf internals
  $PERL -e "use $ac_perl_version;" > /dev/null 2>&1
  if test $? -ne 0; then
    AC_MSG_RESULT(no);
    $3
  else
    AC_MSG_RESULT(ok);
    $2
  fi
else
  AC_MSG_WARN(could not find perl)
fi
])dnl




#######################################################################
# CONDOR_EXTERNAL_VERSION written by Derek Wright
# <wright@cs.wisc.edu> to specify the required version for a given
# external package needed for building Condor.  In addition to setting
# variables and printing out stuff, this macro checks to make sure
# that the externals directory tree we've been configured to use
# contains the given version of the external package.
# Arguments: 
#  $1 is the package "name" for the external
#  $2 is the package "version" of the external
# Side Effects:
#  If the given version is found in the externals tree, 
#  the variable $_cv_ext_<name>_version is set to hold the value, we
#  print out "checking <name> ... <version>, and we do a variable
#  substitution in our output files for "ext_<name>_version".
#######################################################################
AC_DEFUN([CONDOR_EXTERNAL_VERSION],
[CONDOR_EXTERNAL_VERSION_($1,$2,m4_toupper($1))])

AC_DEFUN([CONDOR_EXTERNAL_VERSION_],
[AC_MSG_CHECKING([$1])
 _err_msg="The requested version of [$1] ([$1]-[$2]) does not exist in $ac_cv_externals"
 _condor_VERIFY_DIR([$ac_cv_externals/bundles/[$1]],$_err_msg)
 _condor_VERIFY_DIR([$ac_cv_externals/bundles/[$1]/[$2]],$_err_msg)
 _condor_VERIFY_FILE([$ac_cv_externals/bundles/[$1]/[$2]/build_[$1]-[$2]],$_err_msg)
 _cv_ext_$1_version="[$1]-[$2]"
 AC_MSG_RESULT([$_cv_ext_$1_version])
 AC_SUBST([ext_$1_version],$_cv_ext_$1_version)
 AC_DEFINE([HAVE_EXT_$3],[1],[Do we have the $3 external package])
])


#######################################################################
# CONDOR_VERIFY_EXTERNALS_DIR written by Derek Wright
# <wright@cs.wisc.edu> to verify if a given directory is a valid
# externals directory tree needed for building Condor.  To simplify
# the macro and make the reporting consistent, this macro uses some
# helper macros (_condor_VERITY_*) to check for files and directories
# and report any errors found while trying to verify a given path.
# Arguments: 
#  $1 is the path to test
#  $2 is the error message to report if the given path is invalid
# Side Effects:
#  If the given path is valid, the variable $ac_cv_externals is set to
#  hold its value
#######################################################################
AC_DEFUN([CONDOR_VERIFY_EXTERNALS_DIR],
[AC_MSG_CHECKING([if $1 is valid])
 _condor_VERIFY_DIR([$1], [$2])
 _condor_VERIFY_FILE([$1/build_external], [$2])
 _condor_VERIFY_DIR([$1/bundles], [$2])
 AC_MSG_RESULT(yes)
 ac_cv_externals=$1
])


#######################################################################
# CONDOR_VERIFY_CONFIG_DIR written by Derek Wright
# <wright@cs.wisc.edu> to verify if a given directory is a valid
# config directory tree needed for building Condor.  To simplify
# the macro and make the reporting consistent, this macro uses some
# helper macros (_condor_VERITY_*) to check for files and directories
# and report any errors found while trying to verify a given path.
# Arguments: 
#  $1 is the path to test
#  $2 is the error message to report if the given path is invalid
# Side Effects:
#  If the given path is valid, the variable $ac_cv_config is set to
#  hold its value
#######################################################################
AC_DEFUN([CONDOR_VERIFY_CONFIG_DIR],
[AC_MSG_CHECKING([if $1 is valid])
 _condor_VERIFY_DIR([$1], [$2])
 _condor_VERIFY_FILE([$1/configure.cf.in], [$2])
 _condor_VERIFY_FILE([$1/externals.cf.in], [$2])
 _condor_VERIFY_FILE([$1/config.sh.in], [$2])
 AC_MSG_RESULT(yes)
 ac_cv_config=$1
])


#######################################################################
# _condor_VERIFY_FILE is a helper for the various _condor_VERIFY_*
# macros 
# Arguments:
#  $1 file to test
#  $2 is the general error message to print which was given to
#     _condor_VERIFY_*
# Side Effects:
#   If the given file doesn't exist, this macro calls
#   _condor_VERIFY_ERROR which will terminate with AC_MSG_ERROR
#######################################################################
AC_DEFUN([_condor_VERIFY_FILE],
[if test ! -f "$1"; then
   _condor_VERIFY_ERROR([$1 does not exist], [$2])
 fi
])


#######################################################################
# _condor_VERIFY_DIR is a helper for the _condor_VERIFY_* macros 
# Arguments:
#  $1 directory to test
#  $2 is the general error message to print which was given to
#     _condor_VERIFY_*
# Side Effects:
#   If the given directory doesn't exist, this macro calls
#   _condor_VERIFY_ERROR which will terminate with AC_MSG_ERROR
#######################################################################
AC_DEFUN([_condor_VERIFY_DIR],
[if test ! -d "$1"; then
   _condor_VERIFY_ERROR([$1 is not a directory], [$2])
 fi
])


#######################################################################
# _condor_VERIFY_ERROR is a helper for the _condor_VERIFY_* macros
# Arguments:
#  $1 is the specific warning message about why a directory is invalid
#  $2 is the general error message to print which was given to
#     _condor_VERIFY_*_DIR
# Side Effects:
#  Calling this macro terminates configure with AC_MSG_ERROR
#######################################################################
AC_DEFUN([_condor_VERIFY_ERROR],
[AC_MSG_RESULT([no])
 AC_MSG_WARN([$1])
 AC_MSG_ERROR([$2])
])


#######################################################################
# CONDOR_VERIFY_ANALYZE_DIR by Derek Wright <wright@cs.wisc.edu>
# Verifies if a given directory has valid condor_analyze binaries for
# the Condor release.
# Arguments: 
#  $1 is the path to test
# Side Effects:
#  If the given path is valid, the variable $ac_condor_analyze_dir is
#  set to hold its value, and $ac_has_condor_analyze is set to "YES".
#  Additionally, if the directory contains any condor_analyze binaries
#  (ending in ".static", ".strip" and/or ".release",
#  $ac_condor_analyze_(static|strip|release) are set to the full
#  paths of the corresponding binary.
#######################################################################
AC_DEFUN([CONDOR_VERIFY_ANALYZE_DIR],
[
 ac_has_condor_analyze=NO
 if test -d $1 ; then
   if test -f "$1/condor_analyze.strip" ; then
     ac_condor_analyze_strip="$1/condor_analyze.strip"
     ac_has_condor_analyze=YES
   fi
   if test -f "$1/condor_analyze.static" ; then
     ac_condor_analyze_static="$1/condor_analyze.static"
     ac_has_condor_analyze=YES
   fi
   if test -f "$1/condor_analyze.release" ; then
     ac_condor_analyze_release="$1/condor_analyze.release"
     ac_has_condor_analyze=YES
   fi
   if test -f "$1/condor_analyze" ; then
     ac_condor_analyze="$1/condor_analyze"
     ac_has_condor_analyze=YES
   fi
   if test $ac_has_condor_analyze = "YES" ; then
     ac_condor_analyze_dir=$1
   fi
 fi
])


#######################################################################
# CONDOR_SET_ANALYZE by Derek Wright <wright@cs.wisc.edu> 
# Once we know what binaries we're using for Condor analyze, this
# macro is used as a helper to keep configure.ac more readable.  It is
# responsible for calling the right AC_SUBST() macro, depending on if
# we found the per-type version (release, static, or strip), or just a
# plain "condor_analyze" binary.
# Arguments: 
#  $1 is the keyword used in variable names and filenames
#  $2 a string, either "fatal" or not, to specify if this macro should
#     call AC_MSG_ERROR() if neither the per-type nor generic version
#     of condor_analyze was found
#######################################################################
AC_DEFUN([CONDOR_SET_ANALYZE],
[
 if test "x$[ac_condor_analyze_[$1]]" = "x"; then
   if test ! "x$ac_condor_analyze" = "x" ; then
     AC_MSG_WARN([no condor_analyze.$1 found, using $ac_condor_analyze])
     AC_SUBST([condor_analyze_[$1]],$ac_condor_analyze)
   else
     if test "$2" = "fatal" ; then 
       AC_MSG_ERROR([neither condor_analyze.$1 nor condor_analyze found])
     else
       AC_MSG_WARN([neither condor_analyze.$1 nor condor_analyze found])
     fi
   fi
 else 
   AC_SUBST([condor_analyze_[$1]],$[ac_condor_analyze_[$1]])
 fi
])


#######################################################################
# GET_GCC_VALUE by Derek Wright <wright@cs.wisc.edu> 
# We need to find out a few things from our gcc installation, like the
# full paths to some of the support libraries, etc.  This macro just
# invokes gcc with a given option, saves the value into a variable, and
# if it worked, it substitutes the discovered value.
# Arguments: 
#  $1 the "checking for" message to print
#  $2 the gcc option to envoke
#  $3 the variable name (used for storing in cache with "_cv_"
#     prepended to the front, and for substitution in the output)
#######################################################################
AC_DEFUN([GET_GCC_VALUE],
[AC_CACHE_CHECK( [for $1], [_cv_[$3]],
 [[_cv_$3]=`gcc $2 2>/dev/null`
  _gcc_status=$?
  if test $_gcc_status -ne 0; then
    [_cv_$3]="no";
  fi
 ])
 if test $[_cv_[$3]] = "no"; then
   AC_MSG_RESULT([no])
 else
   AC_MSG_RESULT([yes])
   AC_SUBST($3,$[_cv_[$3]])
 fi
])

#######################################################################
# C_CXX_OPTION by Peter Keller <psilord@cs.wisc.edu>
# We need to see whether or not a particular flag is available for a
# compiler (which should be either a C or C++ compiler).
#  $1 the compiler path to check
#  $2 the warning option to check
#  $3 the variable name (used for storing in cache with "_cv_"
#     prepended to the front, and for substitution in the output)
#  $4 any optional flags which should be passed to gcc to test the flag in
#     question to test the first flag
#######################################################################
AC_DEFUN([C_CXX_OPTION],
[AC_CACHE_CHECK( [if $1 supports $2], [_cv_$3],
cat > conf_flag.c << EOF
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
	return 0;
}
EOF
 # Stupid compilers sometimes say "Invalid option" or "unrecognized option",
 # but they always seem to say "option" in there.... Also sometimes an
 # executable is produced, other times not. So we'll assume that if the word
 # "option" shows up at all in the stderr of the compiler, it is unrecognized.
 [[_cv_$3]=`$1 $2 $4 conf_flag.c -c -o conf_flag.o 2>&1 | grep "option" > /dev/null 2>&1`
  _comp_status=$?
  # the test seems inverted because the failure of grep to find the 
  # sentinel value means gcc accepted it
  if test $_comp_status -eq 1; then
    [_cv_$3]="yes";
  else
    [_cv_$3]="no";
  fi
 ])
 if test $[_cv_$3] = "yes"; then
   AC_SUBST($3,$2)
 fi
  rm -f conf_flag.c conf_flag.o
])

#######################################################################
# CONDOR_TRY_CP_RECURSIVE_SYMLINK_FLAG by <wright@cs.wisc.edu> 
# When packaging Condor, we sometimes need to recursively copy files.
# Hoewver, when we do, we want to dereference symlinks and copy what
# they point to, not copy the links themselves.  Unfortunately, the
# default behavior with GNU cp using "-r" is to copy the links, not 
# dereference them.  So, we check in the configure script what option
# works to both copy recursively and dereference symlinks.  This macro
# is used inside that loop to do the actual testing.  
# Arguments: 
#  $1 the full path to cp to try
#  $2 the command-line option to cp to test
#  $3 the variable name to set to "none" if the flag failed, or to
#     hold the flag itself if it worked.
#######################################################################
AC_DEFUN([CONDOR_TRY_CP_RECURSIVE_SYMLINK_FLAG],
[
 # initialize the flag imagining we're going to fail.  we'll set it to
 # the right thing if the flag really worked
 [$3]="none";
 # beware, at this stage, all we can count on is the configure script
#itself ex
 touch conftest_base > /dev/null 2>&1
 ln -s conftest_base conftest_link > /dev/null 2>&1
 $1 $2 conftest_link conftest_file > /dev/null 2>&1
 if test -f "conftest_file" ; then
   perl -e 'exit -l "conftest_file"' > /dev/null 2>&1
   _is_link=$?
   if test "$_is_link" = "0" ; then
   # it's not a symlink!  this flag will work.
     [$3]="$2";
   fi
 fi
 # either way, clean up our test files
 /bin/rm -f conftest_base conftest_link conftest_file > /dev/null 2>&1
])


#######################################################################
# CONDOR_TEST_STRIP by Derek Wright <wright@cs.wisc.edu> 
# We need to use strip for packaging Condor.  Usually, we prefer GNU
# strip but sometimes we need to use the vendor strip.  So, this macro
# is used for testing a given version of strip to see if it produces
# an output program that still works, and if so, how big the resulting
# output program is in bytes.  We use this macro on both GNU and
# vendor strip (if we find them), and if both versions work, we use
# the size to determine which version to use.
# 
# Unfortunately, this test has to be fairly unorthodox, since it
# doesn't neatly fall into the standard way of testing things.  First
# of all, we can't use autoconf's compiler or link tests, since we
# need to run strip against the output, and all of autoconf's pre-made
# tests clean up after themselves.  Also, we can't really use the
# cache for this, since we want to check 2 values, and we can't do
# that without breaking some cache rules (like no side effects in your
# cache check).  Also, we assume "g++" is in the PATH and works (we
# want to make a c++ test program, since that's what we're going to be
# stripping, anyway).  Therefore, this macro should be called *AFTER*
# we test for g++ 
#
# Arguments
#  $1 identifying message to print 
#  $2 variable name of the strip to test.  not only is this used to
#     invoke strip, it's also used for naming a few other variables
# Side Effects:
#  Sets a few variables with the result of the test:
#    "<variable_name>_works"=(yes|no)  
#    "<variable_name>_outsize"=(size in bytes of the stripped program)
#######################################################################
AC_DEFUN([CONDOR_TEST_STRIP],
[AC_MSG_CHECKING([if $1 works])
 [$2]_works="no"
 [$2]_outsize=""
 cat > conftest.c << _TESTEOF
#include <stdio.h>
int main () {
  printf( "hello world\n" );
  return 0;
}
_TESTEOF
 g++ -g -o conftest1 conftest.c > /dev/null 2>&1
 if test $? -ne 0 ; then
   AC_MSG_RESULT([no])
   AC_MSG_ERROR([failed to build simple gcc test program])
 fi
 $[$2] conftest1 > /dev/null 2>&1
 if test $? -eq 0 &&
      (sh -c ./conftest1 2>/dev/null |grep "hello world" 2>&1 >/dev/null) ;
 then
   [$2]_works="yes"
   [$2]_outsize="`/bin/ls -nl conftest1 2> /dev/null | awk '{print [$]5}'`"
   AC_MSG_RESULT([yes])
 else
   AC_MSG_RESULT([no])
 fi
 rm -f conftest.c conftest1
])


#######################################################################
# REQUIRE_PATH_PROG by Derek Wright <wright@cs.wisc.edu> 
# Performs an AC_PATH_PROG, and then makes sure that we found the
# program we were looking for.
#
# Arguments (just the first two you pass to AC_PATH_PROG)
#  $1 variable name 
#  $2 program name
# Side Effects:
#  Calls AC_PATH_PROG, which does a bunch of stuff for you.
#######################################################################
AC_DEFUN([REQUIRE_PATH_PROG],
[AC_PATH_PROG([$1],[$2],[no],[$PATH])
 if test "$[$1]" = "no" ; then
   AC_MSG_ERROR([$2 is required])
 fi
])


#######################################################################
# CHECK_PATH_PROG by Derek Wright <wright@cs.wisc.edu> 
# Checks for the full path to the requested tool.
# Assumes you already called AC_PATH_PROG(WHICH,which).
# We can't use AC_REQUIRE for that assumption, b/c that only works
# for macros without arguments.
#
# Arguments
#  $1 program name
#  $2 variable name prefix 
# Side Effects:
#  sets $2_path to the full path to $1
#######################################################################
AC_DEFUN([CHECK_PATH_PROG],
[AC_MSG_CHECKING([for full path to $1])
 if test $WHICH != no; then
   $2_path=`$WHICH $1`
   AC_MSG_RESULT($[$2_path])
 else
   AC_MSG_RESULT([this platform does not support 'which'])
 fi
])


dnl $Id: aclocal.m4,v 1.5.26.1 2007-03-06 23:12:55 epaulson Exp $
dnl
dnl @synopsis AX_LIB_ORACLE_OCI([MINIMUM-VERSION])
dnl
dnl This macro provides tests of availability of Oracle OCI API
dnl of particular version or newer.
dnl This macros checks for Oracle OCI headers and libraries 
dnl and defines compilation flags
dnl 
dnl Macro supports following options and their values:
dnl 1) Single-option usage:
dnl --with-oci - path to ORACLE_HOME directory
dnl 2) Two-options usage (both options are required):
dnl --with-oci-include - path to directory with OCI headers
dnl --with-oci-lib - path to directory with OCI libraries 
dnl
dnl NOTE: These options described above does not take yes|no values.
dnl If 'yes' value is passed, then WARNING message will be displayed,
dnl 'no' value, as well as the --without-oci-* variations will cause
dnl the macro won't check enything.
dnl
dnl This macro calls:
dnl
dnl   AC_SUBST(ORACLE_OCI_CFLAGS)
dnl   AC_SUBST(ORACLE_OCI_LDFLAGS)
dnl   AC_SUBST(ORACLE_OCI_VERSION)
dnl
dnl And sets:
dnl
dnl   HAVE_ORACLE_OCI
dnl
dnl @category InstalledPackages
dnl @category Cxx
dnl @author Mateusz Loskot <mateusz@loskot.net>
dnl @version $Date: 2007-03-06 23:12:55 $
dnl @license AllPermissive
dnl
AC_DEFUN([AX_LIB_ORACLE_OCI],
[
    AC_ARG_WITH([oci],
        AC_HELP_STRING([--with-oci=@<:@ARG@:>@],
            [use Oracle OCI API from given Oracle home (ARG=path); use existing ORACLE_HOME (ARG=yes); disable Oracle OCI support (ARG=no)]
        ),
        [
        if test "$withval" = "yes"; then
            if test -n "$ORACLE_HOME"; then
                oracle_home_dir="$ORACLE_HOME"
            else
                oracle_home_dir=""
            fi 
        elif test -d "$withval"; then
            oracle_home_dir="$withval"
        else
            oracle_home_dir=""
        fi
        ],
        [
        if test -n "$ORACLE_HOME"; then
            oracle_home_dir="$ORACLE_HOME"
        else
            oracle_home_dir=""
        fi 
        ]
    )

    AC_ARG_WITH([oci-include],
        AC_HELP_STRING([--with-oci-include=@<:@DIR@:>@],
            [use Oracle OCI API headers from given path]
        ),
        [oracle_home_include_dir="$withval"],
        [oracle_home_include_dir=""]
    )
    AC_ARG_WITH([oci-lib],
        AC_HELP_STRING([--with-oci-lib=@<:@DIR@:>@],
            [use Oracle OCI API libraries from given path]
        ),
        [oracle_home_lib_dir="$withval"],
        [oracle_home_lib_dir=""]
    )

    ORACLE_OCI_CFLAGS=""
    ORACLE_OCI_LDFLAGS=""
    ORACLE_OCI_VERSION=""

    dnl
    dnl Collect include/lib paths
    dnl 
    want_oracle_but_no_path="no"

    if test -n "$oracle_home_dir"; then

        if test "$oracle_home_dir" != "no" -a "$oracle_home_dir" != "yes"; then
            dnl ORACLE_HOME path provided
            oracle_include_dir="$oracle_home_dir/rdbms/public"
            oracle_lib_dir="$oracle_home_dir/lib"
        elif test "$oracle_home_dir" = "yes"; then
            want_oracle_but_no_path="yes"
        fi

    elif test -n "$oracle_home_include_dir" -o -n "$oracle_home_lib_dir"; then

        if test "$oracle_home_include_dir" != "no" -a "$oracle_home_include_dir" != "yes"; then
            oracle_include_dir="$oracle_home_include_dir"
        elif test "$oracle_home_include_dir" = "yes"; then
            want_oracle_but_no_path="yes"
        fi

        if test "$oracle_home_lib_dir" != "no" -a "$oracle_home_lib_dir" != "yes"; then
            oracle_lib_dir="$oracle_home_lib_dir"
        elif test "$oracle_home_lib_dir" = "yes"; then
            want_oracle_but_no_path="yes"
        fi
    fi

    if test "$want_oracle_but_no_path" = "yes"; then
        AC_MSG_WARN([Oracle support is requested but no Oracle paths have been provided. \
Please, locate Oracle directories using --with-oci or \
--with-oci-include and --with-oci-lib options.])
    fi

    dnl
    dnl Check OCI files
    dnl
    if test -n "$oracle_include_dir" -a -n "$oracle_lib_dir"; then

        saved_CPPFLAGS="$CPPFLAGS"
        CPPFLAGS="$CPPFLAGS -I$oracle_include_dir"

        dnl Depending on later Oracle version detection,
        dnl -lnnz10 flag might be removed for older Oracle < 10.x
        saved_LDFLAGS="$LDFLAGS"
        oci_ldflags="-L$oracle_lib_dir -locci -lclntsh"
        LDFLAGS="$LDFLAGS $oci_ldflags -lnnz10"

        dnl
        dnl Check OCI headers
        dnl
        AC_MSG_CHECKING([for Oracle OCI headers in $oracle_include_dir])

        AC_LANG_PUSH(C++)
        AC_COMPILE_IFELSE([
            AC_LANG_PROGRAM([[@%:@include <oci.h>]],
                [[
#if defined(OCI_MAJOR_VERSION)
#if OCI_MAJOR_VERSION == 10 && OCI_MINOR_VERSION == 2
// Oracle 10.2 detected
#endif
#elif defined(OCI_V7_SYNTAX)
// OK, older Oracle detected
// TODO - mloskot: find better macro to check for older versions; 
#else
#  error Oracle oci.h header not found
#endif
                ]]
            )],
            [
            ORACLE_OCI_CFLAGS="-I$oracle_include_dir"
            oci_header_found="yes"
            AC_MSG_RESULT([yes])
            ],
            [
            oci_header_found="no"
            AC_MSG_RESULT([not found])
            ]
        )
        AC_LANG_POP([C++])
        
        dnl
        dnl Check OCI libraries
        dnl
        if test "$oci_header_found" = "yes"; then

            AC_MSG_CHECKING([for Oracle OCI libraries in $oracle_lib_dir])

            AC_LANG_PUSH(C++)
            AC_LINK_IFELSE([
                AC_LANG_PROGRAM([[@%:@include <oci.h>]],
                    [[
OCIEnv* envh = 0;
OCIEnvCreate(&envh, OCI_DEFAULT, 0, 0, 0, 0, 0, 0);
if (envh) OCIHandleFree(envh, OCI_HTYPE_ENV);
                    ]]
                )],
                [
                ORACLE_OCI_LDFLAGS="$oci_ldflags"
                oci_lib_found="yes"
                AC_MSG_RESULT([yes])
                ],
                [
                oci_lib_found="no"
                AC_MSG_RESULT([not found])
                ]
            )
            AC_LANG_POP([C++])
        fi

        CPPFLAGS="$saved_CPPFLAGS"
        LDFLAGS="$saved_LDFLAGS"
    fi

    dnl
    dnl Check required version of Oracle is available
    dnl
    oracle_version_req=ifelse([$1], [], [], [$1])

    if test "$oci_header_found" = "yes" -a "$oci_lib_found" = "yes" -a \
        -n "$oracle_version_req"; then

        oracle_version_major=`cat $oracle_include_dir/oci.h \
                             | grep '#define.*OCI_MAJOR_VERSION.*' \
                             | sed -e 's/#define OCI_MAJOR_VERSION  *//' \
                             | sed -e 's/  *\/\*.*\*\///'`

        oracle_version_minor=`cat $oracle_include_dir/oci.h \
                             | grep '#define.*OCI_MINOR_VERSION.*' \
                             | sed -e 's/#define OCI_MINOR_VERSION  *//' \
                             | sed -e 's/  *\/\*.*\*\///'`

        AC_MSG_CHECKING([if Oracle OCI version is >= $oracle_version_req])

        if test -n "$oracle_version_major" -a -n $"oracle_version_minor"; then

            ORACLE_OCI_VERSION="$oracle_version_major.$oracle_version_minor"

            dnl Decompose required version string of Oracle
            dnl and calculate its number representation
            oracle_version_req_major=`expr $oracle_version_req : '\([[0-9]]*\)'`
            oracle_version_req_minor=`expr $oracle_version_req : '[[0-9]]*\.\([[0-9]]*\)'`

            oracle_version_req_number=`expr $oracle_version_req_major \* 1000000 \
                                       \+ $oracle_version_req_minor \* 1000`

            dnl Calculate its number representation 
            oracle_version_number=`expr $oracle_version_major \* 1000000 \
                                  \+ $oracle_version_minor \* 1000`

            oracle_version_check=`expr $oracle_version_number \>\= $oracle_version_req_number`
            if test "$oracle_version_check" = "1"; then

                oracle_version_checked="yes"
                AC_MSG_RESULT([yes])

                dnl Add -lnnz10 flag to Oracle >= 10.x
                AC_MSG_CHECKING([for Oracle version >= 10.x to use -lnnz10 flag])
                oracle_nnz10_check=`expr $oracle_version_number \>\= 10 \* 1000000`
                if test "$oracle_nnz10_check" = "1"; then
                    ORACLE_OCI_LDFLAGS="$ORACLE_OCI_LDFLAGS -lnnz10"
                    AC_MSG_RESULT([yes])
                else
                    AC_MSG_RESULT([no])
                fi
            else
                oracle_version_checked="no"
                AC_MSG_RESULT([no])
                AC_MSG_ERROR([Oracle $ORACLE_OCI_VERSION found, but required version is $oracle_version_req])
            fi
        else
            ORACLE_OCI_VERSION="UNKNOWN"
            AC_MSG_RESULT([no])
            AC_MSG_WARN([Oracle version unknown, probably OCI older than 10.2 is available])
        fi
    fi

    AC_MSG_CHECKING([if Oracle support is enabled])

    if test "$oci_header_found" = "yes" -a "$oci_lib_found" = "yes"; then

        AC_SUBST([ORACLE_OCI_VERSION])
        AC_SUBST([ORACLE_OCI_CFLAGS])
        AC_SUBST([ORACLE_OCI_LDFLAGS])

        HAVE_ORACLE_OCI="yes"
    else
        HAVE_ORACLE_OCI="no"
    fi
    
    AC_MSG_RESULT([$HAVE_ORACLE_OCI])
])

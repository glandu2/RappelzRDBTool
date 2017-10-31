/* Determine a canonical name for the current locale's character encoding.

   Copyright (C) 2000-2006, 2008-2010 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
   USA.  */

/* Written by Bruno Haible <bruno@clisp.org>.  */

#include <config.h>

/* Specification.  */
#include "localcharset.h"

#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined __APPLE__ && defined __MACH__ && HAVE_LANGINFO_CODESET
# define DARWIN7 /* Darwin 7 or newer, i.e. MacOS X 10.3 or newer */
#endif

#if defined _WIN32 || defined __WIN32__
# define WIN32_NATIVE
#endif

#if defined __EMX__
/* Assume EMX program runs on OS/2, even if compiled under DOS.  */
# ifndef OS2
#  define OS2
# endif
#endif

#if !defined WIN32_NATIVE
# include <unistd.h>
# if HAVE_LANGINFO_CODESET
#  include <langinfo.h>
# else
#  if 0 /* see comment below */
#   include <locale.h>
#  endif
# endif
# ifdef __CYGWIN__
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
# endif
#elif defined WIN32_NATIVE
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif
#if defined OS2
# define INCL_DOS
# include <os2.h>
#endif

#if ENABLE_RELOCATABLE
# include "relocatable.h"
#else
# define relocate(pathname) (pathname)
#endif

/* Get LIBDIR.  */
#ifndef LIBDIR
# include "configmake.h"
#endif

/* Define O_NOFOLLOW to 0 on platforms where it does not exist.  */
#ifndef O_NOFOLLOW
# define O_NOFOLLOW 0
#endif

#if defined _WIN32 || defined __WIN32__ || defined __CYGWIN__ || defined __EMX__ || defined __DJGPP__
  /* Win32, Cygwin, OS/2, DOS */
# define ISSLASH(C) ((C) == '/' || (C) == '\\')
#endif

#ifndef DIRECTORY_SEPARATOR
# define DIRECTORY_SEPARATOR '/'
#endif

#ifndef ISSLASH
# define ISSLASH(C) ((C) == DIRECTORY_SEPARATOR)
#endif

#if HAVE_DECL_GETC_UNLOCKED
# undef getc
# define getc getc_unlocked
#endif

/* The following static variable is declared 'volatile' to avoid a
   possible multithread problem in the function get_charset_aliases. If we
   are running in a threaded environment, and if two threads initialize
   'charset_aliases' simultaneously, both will produce the same value,
   and everything will be ok if the two assignments to 'charset_aliases'
   are atomic. But I don't know what will happen if the two assignments mix.  */
#if __STDC__ != 1
# define volatile /* empty */
#endif
/* Pointer to the contents of the charset.alias file, if it has already been
   read, else NULL.  Its format is:
   ALIAS_1 '\0' CANONICAL_1 '\0' ... ALIAS_n '\0' CANONICAL_n '\0' '\0'  */
static const char * volatile charset_aliases;

/* Return a pointer to the contents of the charset.alias file.  */
static const char *
get_charset_aliases (void)
{
  const char *cp;

  cp = charset_aliases;
  if (cp == NULL)
    {
#if !(defined DARWIN7 || defined VMS || defined WIN32_NATIVE || defined __CYGWIN__ || defined __linux__)
      const char *dir;
      const char *base = "charset.alias";
      char *file_name;

      /* Make it possible to override the charset.alias location.  This is
         necessary for running the testsuite before "make install".  */
      dir = getenv ("CHARSETALIASDIR");
      if (dir == NULL || dir[0] == '\0')
        dir = relocate (LIBDIR);

      /* Concatenate dir and base into freshly allocated file_name.  */
      {
        size_t dir_len = strlen (dir);
        size_t base_len = strlen (base);
        int add_slash = (dir_len > 0 && !ISSLASH (dir[dir_len - 1]));
        file_name = (char *) malloc (dir_len + add_slash + base_len + 1);
        if (file_name != NULL)
          {
            memcpy (file_name, dir, dir_len);
            if (add_slash)
              file_name[dir_len] = DIRECTORY_SEPARATOR;
            memcpy (file_name + dir_len + add_slash, base, base_len + 1);
          }
      }

      if (file_name == NULL)
        /* Out of memory.  Treat the file as empty.  */
        cp = "";
      else
        {
          int fd;

          /* Open the file.  Reject symbolic links on platforms that support
             O_NOFOLLOW.  This is a security feature.  Without it, an attacker
             could retrieve parts of the contents (namely, the tail of the
             first line that starts with "* ") of an arbitrary file by placing
             a symbolic link to that file under the name "charset.alias" in
             some writable directory and defining the environment variable
             CHARSETALIASDIR to point to that directory.  */
          fd = open (file_name,
                     O_RDONLY | (HAVE_WORKING_O_NOFOLLOW ? O_NOFOLLOW : 0));
          if (fd < 0)
            /* File not found.  Treat it as empty.  */
            cp = "";
          else
            {
              FILE *fp;

              fp = fdopen (fd, "r");
              if (fp == NULL)
                {
                  /* Out of memory.  Treat the file as empty.  */
                  close (fd);
                  cp = "";
                }
              else
                {
                  /* Parse the file's contents.  */
                  char *res_ptr = NULL;
                  size_t res_size = 0;

                  for (;;)
                    {
                      int c;
                      char buf1[50+1];
                      char buf2[50+1];
                      size_t l1, l2;
                      char *old_res_ptr;

                      c = getc (fp);
                      if (c == EOF)
                        break;
                      if (c == '\n' || c == ' ' || c == '\t')
                        continue;
                      if (c == '#')
                        {
                          /* Skip comment, to end of line.  */
                          do
                            c = getc (fp);
                          while (!(c == EOF || c == '\n'));
                          if (c == EOF)
                            break;
                          continue;
                        }
                      ungetc (c, fp);
                      if (fscanf (fp, "%50s %50s", buf1, buf2) < 2)
                        break;
                      l1 = strlen (buf1);
                      l2 = strlen (buf2);
                      old_res_ptr = res_ptr;
                      if (res_size == 0)
                        {
                          res_size = l1 + 1 + l2 + 1;
                          res_ptr = (char *) malloc (res_size + 1);
                        }
                      else
                        {
                          res_size += l1 + 1 + l2 + 1;
                          res_ptr = (char *) realloc (res_ptr, res_size + 1);
                        }
                      if (res_ptr == NULL)
                        {
                          /* Out of memory. */
                          res_size = 0;
                          free (old_res_ptr);
                          break;
                        }
                      strcpy (res_ptr + res_size - (l2 + 1) - (l1 + 1), buf1);
                      strcpy (res_ptr + res_size - (l2 + 1), buf2);
                    }
                  fclose (fp);
                  if (res_size == 0)
                    cp = "";
                  else
                    {
                      *(res_ptr + res_size) = '\0';
                      cp = res_ptr;
                    }
                }
            }

          free (file_name);
        }

#else

# if defined DARWIN7
      /* To avoid the trouble of installing a file that is shared by many
         GNU packages -- many packaging systems have problems with this --,
         simply inline the aliases here.  */
      cp = "ISO8859-1" "\0" "ISO-8859-1" "\0"
           "ISO8859-2" "\0" "ISO-8859-2" "\0"
           "ISO8859-4" "\0" "ISO-8859-4" "\0"
           "ISO8859-5" "\0" "ISO-8859-5" "\0"
           "ISO8859-7" "\0" "ISO-8859-7" "\0"
           "ISO8859-9" "\0" "ISO-8859-9" "\0"
           "ISO8859-13" "\0" "ISO-8859-13" "\0"
           "ISO8859-15" "\0" "ISO-8859-15" "\0"
           "KOI8-R" "\0" "KOI8-R" "\0"
           "KOI8-U" "\0" "KOI8-U" "\0"
           "CP866" "\0" "CP866" "\0"
           "CP949" "\0" "CP949" "\0"
           "CP1131" "\0" "CP1131" "\0"
           "CP1251" "\0" "CP1251" "\0"
           "eucCN" "\0" "GB2312" "\0"
           "GB2312" "\0" "GB2312" "\0"
           "eucJP" "\0" "EUC-JP" "\0"
           "eucKR" "\0" "EUC-KR" "\0"
           "Big5" "\0" "BIG5" "\0"
           "Big5HKSCS" "\0" "BIG5-HKSCS" "\0"
           "GBK" "\0" "GBK" "\0"
           "GB18030" "\0" "GB18030" "\0"
           "SJIS" "\0" "SHIFT_JIS" "\0"
           "ARMSCII-8" "\0" "ARMSCII-8" "\0"
           "PT154" "\0" "PT154" "\0"
         /*"ISCII-DEV" "\0" "?" "\0"*/
           "*" "\0" "UTF-8" "\0";
# endif

# if defined VMS
      /* To avoid the troubles of an extra file charset.alias_vms in the
         sources of many GNU packages, simply inline the aliases here.  */
      /* The list of encodings is taken from the OpenVMS 7.3-1 documentation
         "Compaq C Run-Time Library Reference Manual for OpenVMS systems"
         section 10.7 "Handling Different Character Sets".  */
      cp = "ISO8859-1" "\0" "ISO-8859-1" "\0"
           "ISO8859-2" "\0" "ISO-8859-2" "\0"
           "ISO8859-5" "\0" "ISO-8859-5" "\0"
           "ISO8859-7" "\0" "ISO-8859-7" "\0"
           "ISO8859-8" "\0" "ISO-8859-8" "\0"
           "ISO8859-9" "\0" "ISO-8859-9" "\0"
           /* Japanese */
           "eucJP" "\0" "EUC-JP" "\0"
           "SJIS" "\0" "SHIFT_JIS" "\0"
           "DECKANJI" "\0" "DEC-KANJI" "\0"
           "SDECKANJI" "\0" "EUC-JP" "\0"
           /* Chinese */
           "eucTW" "\0" "EUC-TW" "\0"
           "DECHANYU" "\0" "DEC-HANYU" "\0"
           "DECHANZI" "\0" "GB2312" "\0"
           /* Korean */
           "DECKOREAN" "\0" "EUC-KR" "\0";
# endif

# if defined WIN32_NATIVE || defined __CYGWIN__
      /* To avoid the troubles of installing a separate file in the same
         directory as the DLL and of retrieving the DLL's directory at
         runtime, simply inline the aliases here.  */

      cp = "CP936" "\0" "GBK" "\0"
           "CP1361" "\0" "JOHAB" "\0"
           "CP20127" "\0" "ASCII" "\0"
           "CP20866" "\0" "KOI8-R" "\0"
           "CP20936" "\0" "GB2312" "\0"
           "CP21866" "\0" "KOI8-RU" "\0"
           "CP28591" "\0" "ISO-8859-1" "\0"
           "CP28592" "\0" "ISO-8859-2" "\0"
           "CP28593" "\0" "ISO-8859-3" "\0"
           "CP28594" "\0" "ISO-8859-4" "\0"
           "CP28595" "\0" "ISO-8859-5" "\0"
           "CP28596" "\0" "ISO-8859-6" "\0"
           "CP28597" "\0" "ISO-8859-7" "\0"
           "CP28598" "\0" "ISO-8859-8" "\0"
           "CP28599" "\0" "ISO-8859-9" "\0"
           "CP28605" "\0" "ISO-8859-15" "\0"
           "CP38598" "\0" "ISO-8859-8" "\0"
           "CP51932" "\0" "EUC-JP" "\0"
           "CP51936" "\0" "GB2312" "\0"
           "CP51949" "\0" "EUC-KR" "\0"
           "CP51950" "\0" "EUC-TW" "\0"
           "CP54936" "\0" "GB18030" "\0"
           "CP65001" "\0" "UTF-8" "\0";
# endif

# if defined __linux__
	cp =
	"C" "\0" "ASCII" "\0"
	"POSIX" "\0" "ASCII" "\0"
	"af" "\0" "ISO-8859-1" "\0"
	"af.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"af.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"af.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"af@euro" "\0" "ISO-8859-15" "\0"
	"af.cp-437" "\0" "CP437" "\0"
	"af.cp-850" "\0" "CP850" "\0"
	"af.cp-1252" "\0" "CP1252" "\0"
	"af.cp-1252@euro" "\0" "CP1252" "\0"
	"af.utf-8" "\0" "UTF-8" "\0"
	"af.utf-8@euro" "\0" "UTF-8" "\0"
	"af_ZA" "\0" "ISO-8859-1" "\0"
	"af_ZA.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"af_ZA.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"af_ZA.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"af_ZA@euro" "\0" "ISO-8859-15" "\0"
	"af_ZA.cp-437" "\0" "CP437" "\0"
	"af_ZA.cp-850" "\0" "CP850" "\0"
	"af_ZA.cp-1252" "\0" "CP1252" "\0"
	"af_ZA.cp-1252@euro" "\0" "CP1252" "\0"
	"af_ZA.utf-8" "\0" "UTF-8" "\0"
	"af_ZA.utf-8@euro" "\0" "UTF-8" "\0"
	"ca" "\0" "ISO-8859-1" "\0"
	"ca.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"ca.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"ca.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"ca@euro" "\0" "ISO-8859-15" "\0"
	"ca.cp-437" "\0" "CP437" "\0"
	"ca.cp-850" "\0" "CP850" "\0"
	"ca.cp-1252" "\0" "CP1252" "\0"
	"ca.cp-1252@euro" "\0" "CP1252" "\0"
	"ca.utf-8" "\0" "UTF-8" "\0"
	"ca.utf-8@euro" "\0" "UTF-8" "\0"
	"ca_ES" "\0" "ISO-8859-1" "\0"
	"ca_ES.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"ca_ES.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"ca_ES.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"ca_ES@euro" "\0" "ISO-8859-15" "\0"
	"ca_ES.cp-437" "\0" "CP437" "\0"
	"ca_ES.cp-850" "\0" "CP850" "\0"
	"ca_ES.cp-1252" "\0" "CP1252" "\0"
	"ca_ES.cp-1252@euro" "\0" "CP1252" "\0"
	"ca_ES.utf-8" "\0" "UTF-8" "\0"
	"ca_ES.utf-8@euro" "\0" "UTF-8" "\0"
	"da" "\0" "ISO-8859-1" "\0"
	"da.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"da.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"da.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"da@euro" "\0" "ISO-8859-15" "\0"
	"da.cp-437" "\0" "CP437" "\0"
	"da.cp-850" "\0" "CP850" "\0"
	"da.cp-1252" "\0" "CP1252" "\0"
	"da.cp-1252@euro" "\0" "CP1252" "\0"
	"da.utf-8" "\0" "UTF-8" "\0"
	"da.utf-8@euro" "\0" "UTF-8" "\0"
	"da_DK" "\0" "ISO-8859-1" "\0"
	"da_DK.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"da_DK.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"da_DK.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"da_DK@euro" "\0" "ISO-8859-15" "\0"
	"da_DK.cp-437" "\0" "CP437" "\0"
	"da_DK.cp-850" "\0" "CP850" "\0"
	"da_DK.cp-1252" "\0" "CP1252" "\0"
	"da_DK.cp-1252@euro" "\0" "CP1252" "\0"
	"da_DK.utf-8" "\0" "UTF-8" "\0"
	"da_DK.utf-8@euro" "\0" "UTF-8" "\0"
	"de" "\0" "ISO-8859-1" "\0"
	"de.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"de.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"de.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"de@euro" "\0" "ISO-8859-15" "\0"
	"de.cp-437" "\0" "CP437" "\0"
	"de.cp-850" "\0" "CP850" "\0"
	"de.cp-1252" "\0" "CP1252" "\0"
	"de.cp-1252@euro" "\0" "CP1252" "\0"
	"de.utf-8" "\0" "UTF-8" "\0"
	"de.utf-8@euro" "\0" "UTF-8" "\0"
	"de_AT" "\0" "ISO-8859-1" "\0"
	"de_AT.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"de_AT.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"de_AT.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"de_AT@euro" "\0" "ISO-8859-15" "\0"
	"de_AT.cp-437" "\0" "CP437" "\0"
	"de_AT.cp-850" "\0" "CP850" "\0"
	"de_AT.cp-1252" "\0" "CP1252" "\0"
	"de_AT.cp-1252@euro" "\0" "CP1252" "\0"
	"de_AT.utf-8" "\0" "UTF-8" "\0"
	"de_AT.utf-8@euro" "\0" "UTF-8" "\0"
	"de_BE" "\0" "ISO-8859-1" "\0"
	"de_BE.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"de_BE.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"de_BE.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"de_BE@euro" "\0" "ISO-8859-15" "\0"
	"de_BE.cp-437" "\0" "CP437" "\0"
	"de_BE.cp-850" "\0" "CP850" "\0"
	"de_BE.cp-1252" "\0" "CP1252" "\0"
	"de_BE.cp-1252@euro" "\0" "CP1252" "\0"
	"de_BE.utf-8" "\0" "UTF-8" "\0"
	"de_BE.utf-8@euro" "\0" "UTF-8" "\0"
	"de_CH" "\0" "ISO-8859-1" "\0"
	"de_CH.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"de_CH.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"de_CH.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"de_CH@euro" "\0" "ISO-8859-15" "\0"
	"de_CH.cp-437" "\0" "CP437" "\0"
	"de_CH.cp-850" "\0" "CP850" "\0"
	"de_CH.cp-1252" "\0" "CP1252" "\0"
	"de_CH.cp-1252@euro" "\0" "CP1252" "\0"
	"de_CH.utf-8" "\0" "UTF-8" "\0"
	"de_CH.utf-8@euro" "\0" "UTF-8" "\0"
	"de_DE" "\0" "ISO-8859-1" "\0"
	"de_DE.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"de_DE.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"de_DE.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"de_DE@euro" "\0" "ISO-8859-15" "\0"
	"de_DE.cp-437" "\0" "CP437" "\0"
	"de_DE.cp-850" "\0" "CP850" "\0"
	"de_DE.cp-1252" "\0" "CP1252" "\0"
	"de_DE.cp-1252@euro" "\0" "CP1252" "\0"
	"de_DE.utf-8" "\0" "UTF-8" "\0"
	"de_DE.utf-8@euro" "\0" "UTF-8" "\0"
	"de_LU" "\0" "ISO-8859-1" "\0"
	"de_LU.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"de_LU.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"de_LU.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"de_LU@euro" "\0" "ISO-8859-15" "\0"
	"de_LU.cp-437" "\0" "CP437" "\0"
	"de_LU.cp-850" "\0" "CP850" "\0"
	"de_LU.cp-1252" "\0" "CP1252" "\0"
	"de_LU.cp-1252@euro" "\0" "CP1252" "\0"
	"de_LU.utf-8" "\0" "UTF-8" "\0"
	"de_LU.utf-8@euro" "\0" "UTF-8" "\0"
	"en" "\0" "ISO-8859-1" "\0"
	"en.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"en.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"en.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"en@euro" "\0" "ISO-8859-15" "\0"
	"en.cp-437" "\0" "CP437" "\0"
	"en.cp-850" "\0" "CP850" "\0"
	"en.cp-1252" "\0" "CP1252" "\0"
	"en.cp-1252@euro" "\0" "CP1252" "\0"
	"en.utf-8" "\0" "UTF-8" "\0"
	"en.utf-8@euro" "\0" "UTF-8" "\0"
	"en_AU" "\0" "ISO-8859-1" "\0"
	"en_AU.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"en_AU.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"en_AU.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"en_AU@euro" "\0" "ISO-8859-15" "\0"
	"en_AU.cp-437" "\0" "CP437" "\0"
	"en_AU.cp-850" "\0" "CP850" "\0"
	"en_AU.cp-1252" "\0" "CP1252" "\0"
	"en_AU.cp-1252@euro" "\0" "CP1252" "\0"
	"en_AU.utf-8" "\0" "UTF-8" "\0"
	"en_AU.utf-8@euro" "\0" "UTF-8" "\0"
	"en_BW" "\0" "ISO-8859-1" "\0"
	"en_BW.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"en_BW.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"en_BW.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"en_BW@euro" "\0" "ISO-8859-15" "\0"
	"en_BW.cp-437" "\0" "CP437" "\0"
	"en_BW.cp-850" "\0" "CP850" "\0"
	"en_BW.cp-1252" "\0" "CP1252" "\0"
	"en_BW.cp-1252@euro" "\0" "CP1252" "\0"
	"en_BW.utf-8" "\0" "UTF-8" "\0"
	"en_BW.utf-8@euro" "\0" "UTF-8" "\0"
	"en_CA" "\0" "ISO-8859-1" "\0"
	"en_CA.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"en_CA.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"en_CA.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"en_CA@euro" "\0" "ISO-8859-15" "\0"
	"en_CA.cp-437" "\0" "CP437" "\0"
	"en_CA.cp-850" "\0" "CP850" "\0"
	"en_CA.cp-1252" "\0" "CP1252" "\0"
	"en_CA.cp-1252@euro" "\0" "CP1252" "\0"
	"en_CA.utf-8" "\0" "UTF-8" "\0"
	"en_CA.utf-8@euro" "\0" "UTF-8" "\0"
	"en_DK" "\0" "ISO-8859-1" "\0"
	"en_DK.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"en_DK.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"en_DK.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"en_DK@euro" "\0" "ISO-8859-15" "\0"
	"en_DK.cp-437" "\0" "CP437" "\0"
	"en_DK.cp-850" "\0" "CP850" "\0"
	"en_DK.cp-1252" "\0" "CP1252" "\0"
	"en_DK.cp-1252@euro" "\0" "CP1252" "\0"
	"en_DK.utf-8" "\0" "UTF-8" "\0"
	"en_DK.utf-8@euro" "\0" "UTF-8" "\0"
	"en_GB" "\0" "ISO-8859-1" "\0"
	"en_GB.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"en_GB.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"en_GB.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"en_GB@euro" "\0" "ISO-8859-15" "\0"
	"en_GB.cp-437" "\0" "CP437" "\0"
	"en_GB.cp-850" "\0" "CP850" "\0"
	"en_GB.cp-1252" "\0" "CP1252" "\0"
	"en_GB.cp-1252@euro" "\0" "CP1252" "\0"
	"en_GB.utf-8" "\0" "UTF-8" "\0"
	"en_GB.utf-8@euro" "\0" "UTF-8" "\0"
	"en_IE" "\0" "ISO-8859-1" "\0"
	"en_IE.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"en_IE.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"en_IE.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"en_IE@euro" "\0" "ISO-8859-15" "\0"
	"en_IE.cp-437" "\0" "CP437" "\0"
	"en_IE.cp-850" "\0" "CP850" "\0"
	"en_IE.cp-1252" "\0" "CP1252" "\0"
	"en_IE.cp-1252@euro" "\0" "CP1252" "\0"
	"en_IE.utf-8" "\0" "UTF-8" "\0"
	"en_IE.utf-8@euro" "\0" "UTF-8" "\0"
	"en_NZ" "\0" "ISO-8859-1" "\0"
	"en_NZ.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"en_NZ.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"en_NZ.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"en_NZ@euro" "\0" "ISO-8859-15" "\0"
	"en_NZ.cp-437" "\0" "CP437" "\0"
	"en_NZ.cp-850" "\0" "CP850" "\0"
	"en_NZ.cp-1252" "\0" "CP1252" "\0"
	"en_NZ.cp-1252@euro" "\0" "CP1252" "\0"
	"en_NZ.utf-8" "\0" "UTF-8" "\0"
	"en_NZ.utf-8@euro" "\0" "UTF-8" "\0"
	"en_US" "\0" "ISO-8859-1" "\0"
	"en_US.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"en_US.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"en_US.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"en_US@euro" "\0" "ISO-8859-15" "\0"
	"en_US.cp-437" "\0" "CP437" "\0"
	"en_US.cp-850" "\0" "CP850" "\0"
	"en_US.cp-1252" "\0" "CP1252" "\0"
	"en_US.cp-1252@euro" "\0" "CP1252" "\0"
	"en_US.utf-8" "\0" "UTF-8" "\0"
	"en_US.utf-8@euro" "\0" "UTF-8" "\0"
	"en_ZA" "\0" "ISO-8859-1" "\0"
	"en_ZA.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"en_ZA.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"en_ZA.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"en_ZA@euro" "\0" "ISO-8859-15" "\0"
	"en_ZA.cp-437" "\0" "CP437" "\0"
	"en_ZA.cp-850" "\0" "CP850" "\0"
	"en_ZA.cp-1252" "\0" "CP1252" "\0"
	"en_ZA.cp-1252@euro" "\0" "CP1252" "\0"
	"en_ZA.utf-8" "\0" "UTF-8" "\0"
	"en_ZA.utf-8@euro" "\0" "UTF-8" "\0"
	"en_ZW" "\0" "ISO-8859-1" "\0"
	"en_ZW.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"en_ZW.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"en_ZW.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"en_ZW@euro" "\0" "ISO-8859-15" "\0"
	"en_ZW.cp-437" "\0" "CP437" "\0"
	"en_ZW.cp-850" "\0" "CP850" "\0"
	"en_ZW.cp-1252" "\0" "CP1252" "\0"
	"en_ZW.cp-1252@euro" "\0" "CP1252" "\0"
	"en_ZW.utf-8" "\0" "UTF-8" "\0"
	"en_ZW.utf-8@euro" "\0" "UTF-8" "\0"
	"es" "\0" "ISO-8859-1" "\0"
	"es.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es@euro" "\0" "ISO-8859-15" "\0"
	"es.cp-437" "\0" "CP437" "\0"
	"es.cp-850" "\0" "CP850" "\0"
	"es.cp-1252" "\0" "CP1252" "\0"
	"es.cp-1252@euro" "\0" "CP1252" "\0"
	"es.utf-8" "\0" "UTF-8" "\0"
	"es.utf-8@euro" "\0" "UTF-8" "\0"
	"es_AR" "\0" "ISO-8859-1" "\0"
	"es_AR.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_AR.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_AR.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_AR@euro" "\0" "ISO-8859-15" "\0"
	"es_AR.cp-437" "\0" "CP437" "\0"
	"es_AR.cp-850" "\0" "CP850" "\0"
	"es_AR.cp-1252" "\0" "CP1252" "\0"
	"es_AR.cp-1252@euro" "\0" "CP1252" "\0"
	"es_AR.utf-8" "\0" "UTF-8" "\0"
	"es_AR.utf-8@euro" "\0" "UTF-8" "\0"
	"es_BO" "\0" "ISO-8859-1" "\0"
	"es_BO.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_BO.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_BO.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_BO@euro" "\0" "ISO-8859-15" "\0"
	"es_BO.cp-437" "\0" "CP437" "\0"
	"es_BO.cp-850" "\0" "CP850" "\0"
	"es_BO.cp-1252" "\0" "CP1252" "\0"
	"es_BO.cp-1252@euro" "\0" "CP1252" "\0"
	"es_BO.utf-8" "\0" "UTF-8" "\0"
	"es_BO.utf-8@euro" "\0" "UTF-8" "\0"
	"es_CL" "\0" "ISO-8859-1" "\0"
	"es_CL.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_CL.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_CL.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_CL@euro" "\0" "ISO-8859-15" "\0"
	"es_CL.cp-437" "\0" "CP437" "\0"
	"es_CL.cp-850" "\0" "CP850" "\0"
	"es_CL.cp-1252" "\0" "CP1252" "\0"
	"es_CL.cp-1252@euro" "\0" "CP1252" "\0"
	"es_CL.utf-8" "\0" "UTF-8" "\0"
	"es_CL.utf-8@euro" "\0" "UTF-8" "\0"
	"es_CO" "\0" "ISO-8859-1" "\0"
	"es_CO.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_CO.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_CO.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_CO@euro" "\0" "ISO-8859-15" "\0"
	"es_CO.cp-437" "\0" "CP437" "\0"
	"es_CO.cp-850" "\0" "CP850" "\0"
	"es_CO.cp-1252" "\0" "CP1252" "\0"
	"es_CO.cp-1252@euro" "\0" "CP1252" "\0"
	"es_CO.utf-8" "\0" "UTF-8" "\0"
	"es_CO.utf-8@euro" "\0" "UTF-8" "\0"
	"es_DO" "\0" "ISO-8859-1" "\0"
	"es_DO.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_DO.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_DO.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_DO@euro" "\0" "ISO-8859-15" "\0"
	"es_DO.cp-437" "\0" "CP437" "\0"
	"es_DO.cp-850" "\0" "CP850" "\0"
	"es_DO.cp-1252" "\0" "CP1252" "\0"
	"es_DO.cp-1252@euro" "\0" "CP1252" "\0"
	"es_DO.utf-8" "\0" "UTF-8" "\0"
	"es_DO.utf-8@euro" "\0" "UTF-8" "\0"
	"es_EC" "\0" "ISO-8859-1" "\0"
	"es_EC.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_EC.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_EC.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_EC@euro" "\0" "ISO-8859-15" "\0"
	"es_EC.cp-437" "\0" "CP437" "\0"
	"es_EC.cp-850" "\0" "CP850" "\0"
	"es_EC.cp-1252" "\0" "CP1252" "\0"
	"es_EC.cp-1252@euro" "\0" "CP1252" "\0"
	"es_EC.utf-8" "\0" "UTF-8" "\0"
	"es_EC.utf-8@euro" "\0" "UTF-8" "\0"
	"es_ES" "\0" "ISO-8859-1" "\0"
	"es_ES.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_ES.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_ES.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_ES@euro" "\0" "ISO-8859-15" "\0"
	"es_ES.cp-437" "\0" "CP437" "\0"
	"es_ES.cp-850" "\0" "CP850" "\0"
	"es_ES.cp-1252" "\0" "CP1252" "\0"
	"es_ES.cp-1252@euro" "\0" "CP1252" "\0"
	"es_ES.utf-8" "\0" "UTF-8" "\0"
	"es_ES.utf-8@euro" "\0" "UTF-8" "\0"
	"es_GT" "\0" "ISO-8859-1" "\0"
	"es_GT.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_GT.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_GT.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_GT@euro" "\0" "ISO-8859-15" "\0"
	"es_GT.cp-437" "\0" "CP437" "\0"
	"es_GT.cp-850" "\0" "CP850" "\0"
	"es_GT.cp-1252" "\0" "CP1252" "\0"
	"es_GT.cp-1252@euro" "\0" "CP1252" "\0"
	"es_GT.utf-8" "\0" "UTF-8" "\0"
	"es_GT.utf-8@euro" "\0" "UTF-8" "\0"
	"es_HN" "\0" "ISO-8859-1" "\0"
	"es_HN.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_HN.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_HN.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_HN@euro" "\0" "ISO-8859-15" "\0"
	"es_HN.cp-437" "\0" "CP437" "\0"
	"es_HN.cp-850" "\0" "CP850" "\0"
	"es_HN.cp-1252" "\0" "CP1252" "\0"
	"es_HN.cp-1252@euro" "\0" "CP1252" "\0"
	"es_HN.utf-8" "\0" "UTF-8" "\0"
	"es_HN.utf-8@euro" "\0" "UTF-8" "\0"
	"es_MX" "\0" "ISO-8859-1" "\0"
	"es_MX.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_MX.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_MX.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_MX@euro" "\0" "ISO-8859-15" "\0"
	"es_MX.cp-437" "\0" "CP437" "\0"
	"es_MX.cp-850" "\0" "CP850" "\0"
	"es_MX.cp-1252" "\0" "CP1252" "\0"
	"es_MX.cp-1252@euro" "\0" "CP1252" "\0"
	"es_MX.utf-8" "\0" "UTF-8" "\0"
	"es_MX.utf-8@euro" "\0" "UTF-8" "\0"
	"es_PA" "\0" "ISO-8859-1" "\0"
	"es_PA.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_PA.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_PA.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_PA@euro" "\0" "ISO-8859-15" "\0"
	"es_PA.cp-437" "\0" "CP437" "\0"
	"es_PA.cp-850" "\0" "CP850" "\0"
	"es_PA.cp-1252" "\0" "CP1252" "\0"
	"es_PA.cp-1252@euro" "\0" "CP1252" "\0"
	"es_PA.utf-8" "\0" "UTF-8" "\0"
	"es_PA.utf-8@euro" "\0" "UTF-8" "\0"
	"es_PE" "\0" "ISO-8859-1" "\0"
	"es_PE.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_PE.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_PE.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_PE@euro" "\0" "ISO-8859-15" "\0"
	"es_PE.cp-437" "\0" "CP437" "\0"
	"es_PE.cp-850" "\0" "CP850" "\0"
	"es_PE.cp-1252" "\0" "CP1252" "\0"
	"es_PE.cp-1252@euro" "\0" "CP1252" "\0"
	"es_PE.utf-8" "\0" "UTF-8" "\0"
	"es_PE.utf-8@euro" "\0" "UTF-8" "\0"
	"es_PY" "\0" "ISO-8859-1" "\0"
	"es_PY.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_PY.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_PY.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_PY@euro" "\0" "ISO-8859-15" "\0"
	"es_PY.cp-437" "\0" "CP437" "\0"
	"es_PY.cp-850" "\0" "CP850" "\0"
	"es_PY.cp-1252" "\0" "CP1252" "\0"
	"es_PY.cp-1252@euro" "\0" "CP1252" "\0"
	"es_PY.utf-8" "\0" "UTF-8" "\0"
	"es_PY.utf-8@euro" "\0" "UTF-8" "\0"
	"es_SV" "\0" "ISO-8859-1" "\0"
	"es_SV.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_SV.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_SV.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_SV@euro" "\0" "ISO-8859-15" "\0"
	"es_SV.cp-437" "\0" "CP437" "\0"
	"es_SV.cp-850" "\0" "CP850" "\0"
	"es_SV.cp-1252" "\0" "CP1252" "\0"
	"es_SV.cp-1252@euro" "\0" "CP1252" "\0"
	"es_SV.utf-8" "\0" "UTF-8" "\0"
	"es_SV.utf-8@euro" "\0" "UTF-8" "\0"
	"es_US" "\0" "ISO-8859-1" "\0"
	"es_US.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_US.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_US.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_US@euro" "\0" "ISO-8859-15" "\0"
	"es_US.cp-437" "\0" "CP437" "\0"
	"es_US.cp-850" "\0" "CP850" "\0"
	"es_US.cp-1252" "\0" "CP1252" "\0"
	"es_US.cp-1252@euro" "\0" "CP1252" "\0"
	"es_US.utf-8" "\0" "UTF-8" "\0"
	"es_US.utf-8@euro" "\0" "UTF-8" "\0"
	"es_UY" "\0" "ISO-8859-1" "\0"
	"es_UY.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_UY.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_UY.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_UY@euro" "\0" "ISO-8859-15" "\0"
	"es_UY.cp-437" "\0" "CP437" "\0"
	"es_UY.cp-850" "\0" "CP850" "\0"
	"es_UY.cp-1252" "\0" "CP1252" "\0"
	"es_UY.cp-1252@euro" "\0" "CP1252" "\0"
	"es_UY.utf-8" "\0" "UTF-8" "\0"
	"es_UY.utf-8@euro" "\0" "UTF-8" "\0"
	"es_VE" "\0" "ISO-8859-1" "\0"
	"es_VE.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"es_VE.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"es_VE.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"es_VE@euro" "\0" "ISO-8859-15" "\0"
	"es_VE.cp-437" "\0" "CP437" "\0"
	"es_VE.cp-850" "\0" "CP850" "\0"
	"es_VE.cp-1252" "\0" "CP1252" "\0"
	"es_VE.cp-1252@euro" "\0" "CP1252" "\0"
	"es_VE.utf-8" "\0" "UTF-8" "\0"
	"es_VE.utf-8@euro" "\0" "UTF-8" "\0"
	"et" "\0" "ISO-8859-1" "\0"
	"et.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"et.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"et.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"et@euro" "\0" "ISO-8859-15" "\0"
	"et.cp-437" "\0" "CP437" "\0"
	"et.cp-850" "\0" "CP850" "\0"
	"et.cp-1252" "\0" "CP1252" "\0"
	"et.cp-1252@euro" "\0" "CP1252" "\0"
	"et.utf-8" "\0" "UTF-8" "\0"
	"et.utf-8@euro" "\0" "UTF-8" "\0"
	"et_EE" "\0" "ISO-8859-1" "\0"
	"et_EE.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"et_EE.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"et_EE.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"et_EE@euro" "\0" "ISO-8859-15" "\0"
	"et_EE.cp-437" "\0" "CP437" "\0"
	"et_EE.cp-850" "\0" "CP850" "\0"
	"et_EE.cp-1252" "\0" "CP1252" "\0"
	"et_EE.cp-1252@euro" "\0" "CP1252" "\0"
	"et_EE.utf-8" "\0" "UTF-8" "\0"
	"et_EE.utf-8@euro" "\0" "UTF-8" "\0"
	"eu" "\0" "ISO-8859-1" "\0"
	"eu.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"eu.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"eu.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"eu@euro" "\0" "ISO-8859-15" "\0"
	"eu.cp-437" "\0" "CP437" "\0"
	"eu.cp-850" "\0" "CP850" "\0"
	"eu.cp-1252" "\0" "CP1252" "\0"
	"eu.cp-1252@euro" "\0" "CP1252" "\0"
	"eu.utf-8" "\0" "UTF-8" "\0"
	"eu.utf-8@euro" "\0" "UTF-8" "\0"
	"eu_ES" "\0" "ISO-8859-1" "\0"
	"eu_ES.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"eu_ES.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"eu_ES.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"eu_ES@euro" "\0" "ISO-8859-15" "\0"
	"eu_ES.cp-437" "\0" "CP437" "\0"
	"eu_ES.cp-850" "\0" "CP850" "\0"
	"eu_ES.cp-1252" "\0" "CP1252" "\0"
	"eu_ES.cp-1252@euro" "\0" "CP1252" "\0"
	"eu_ES.utf-8" "\0" "UTF-8" "\0"
	"eu_ES.utf-8@euro" "\0" "UTF-8" "\0"
	"fi" "\0" "ISO-8859-1" "\0"
	"fi.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"fi.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"fi.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"fi@euro" "\0" "ISO-8859-15" "\0"
	"fi.cp-437" "\0" "CP437" "\0"
	"fi.cp-850" "\0" "CP850" "\0"
	"fi.cp-1252" "\0" "CP1252" "\0"
	"fi.cp-1252@euro" "\0" "CP1252" "\0"
	"fi.utf-8" "\0" "UTF-8" "\0"
	"fi.utf-8@euro" "\0" "UTF-8" "\0"
	"fi_FI" "\0" "ISO-8859-1" "\0"
	"fi_FI.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"fi_FI.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"fi_FI.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"fi_FI@euro" "\0" "ISO-8859-15" "\0"
	"fi_FI.cp-437" "\0" "CP437" "\0"
	"fi_FI.cp-850" "\0" "CP850" "\0"
	"fi_FI.cp-1252" "\0" "CP1252" "\0"
	"fi_FI.cp-1252@euro" "\0" "CP1252" "\0"
	"fi_FI.utf-8" "\0" "UTF-8" "\0"
	"fi_FI.utf-8@euro" "\0" "UTF-8" "\0"
	"fo" "\0" "ISO-8859-1" "\0"
	"fo.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"fo.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"fo.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"fo@euro" "\0" "ISO-8859-15" "\0"
	"fo.cp-437" "\0" "CP437" "\0"
	"fo.cp-850" "\0" "CP850" "\0"
	"fo.cp-1252" "\0" "CP1252" "\0"
	"fo.cp-1252@euro" "\0" "CP1252" "\0"
	"fo.utf-8" "\0" "UTF-8" "\0"
	"fo.utf-8@euro" "\0" "UTF-8" "\0"
	"fo_FO" "\0" "ISO-8859-1" "\0"
	"fo_FO.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"fo_FO.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"fo_FO.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"fo_FO@euro" "\0" "ISO-8859-15" "\0"
	"fo_FO.cp-437" "\0" "CP437" "\0"
	"fo_FO.cp-850" "\0" "CP850" "\0"
	"fo_FO.cp-1252" "\0" "CP1252" "\0"
	"fo_FO.cp-1252@euro" "\0" "CP1252" "\0"
	"fo_FO.utf-8" "\0" "UTF-8" "\0"
	"fo_FO.utf-8@euro" "\0" "UTF-8" "\0"
	"fr" "\0" "ISO-8859-1" "\0"
	"fr.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"fr.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"fr.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"fr@euro" "\0" "ISO-8859-15" "\0"
	"fr.cp-437" "\0" "CP437" "\0"
	"fr.cp-850" "\0" "CP850" "\0"
	"fr.cp-1252" "\0" "CP1252" "\0"
	"fr.cp-1252@euro" "\0" "CP1252" "\0"
	"fr.utf-8" "\0" "UTF-8" "\0"
	"fr.utf-8@euro" "\0" "UTF-8" "\0"
	"fr_BE" "\0" "ISO-8859-1" "\0"
	"fr_BE.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"fr_BE.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"fr_BE.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"fr_BE@euro" "\0" "ISO-8859-15" "\0"
	"fr_BE.cp-437" "\0" "CP437" "\0"
	"fr_BE.cp-850" "\0" "CP850" "\0"
	"fr_BE.cp-1252" "\0" "CP1252" "\0"
	"fr_BE.cp-1252@euro" "\0" "CP1252" "\0"
	"fr_BE.utf-8" "\0" "UTF-8" "\0"
	"fr_BE.utf-8@euro" "\0" "UTF-8" "\0"
	"fr_CA" "\0" "ISO-8859-1" "\0"
	"fr_CA.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"fr_CA.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"fr_CA.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"fr_CA@euro" "\0" "ISO-8859-15" "\0"
	"fr_CA.cp-437" "\0" "CP437" "\0"
	"fr_CA.cp-850" "\0" "CP850" "\0"
	"fr_CA.cp-1252" "\0" "CP1252" "\0"
	"fr_CA.cp-1252@euro" "\0" "CP1252" "\0"
	"fr_CA.utf-8" "\0" "UTF-8" "\0"
	"fr_CA.utf-8@euro" "\0" "UTF-8" "\0"
	"fr_CH" "\0" "ISO-8859-1" "\0"
	"fr_CH.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"fr_CH.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"fr_CH.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"fr_CH@euro" "\0" "ISO-8859-15" "\0"
	"fr_CH.cp-437" "\0" "CP437" "\0"
	"fr_CH.cp-850" "\0" "CP850" "\0"
	"fr_CH.cp-1252" "\0" "CP1252" "\0"
	"fr_CH.cp-1252@euro" "\0" "CP1252" "\0"
	"fr_CH.utf-8" "\0" "UTF-8" "\0"
	"fr_CH.utf-8@euro" "\0" "UTF-8" "\0"
	"fr_FR" "\0" "ISO-8859-1" "\0"
	"fr_FR.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"fr_FR.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"fr_FR.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"fr_FR@euro" "\0" "ISO-8859-15" "\0"
	"fr_FR.cp-437" "\0" "CP437" "\0"
	"fr_FR.cp-850" "\0" "CP850" "\0"
	"fr_FR.cp-1252" "\0" "CP1252" "\0"
	"fr_FR.cp-1252@euro" "\0" "CP1252" "\0"
	"fr_FR.utf-8" "\0" "UTF-8" "\0"
	"fr_FR.utf-8@euro" "\0" "UTF-8" "\0"
	"fr_LU" "\0" "ISO-8859-1" "\0"
	"fr_LU.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"fr_LU.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"fr_LU.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"fr_LU@euro" "\0" "ISO-8859-15" "\0"
	"fr_LU.cp-437" "\0" "CP437" "\0"
	"fr_LU.cp-850" "\0" "CP850" "\0"
	"fr_LU.cp-1252" "\0" "CP1252" "\0"
	"fr_LU.cp-1252@euro" "\0" "CP1252" "\0"
	"fr_LU.utf-8" "\0" "UTF-8" "\0"
	"fr_LU.utf-8@euro" "\0" "UTF-8" "\0"
	"ga" "\0" "ISO-8859-1" "\0"
	"ga.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"ga.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"ga.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"ga@euro" "\0" "ISO-8859-15" "\0"
	"ga.cp-437" "\0" "CP437" "\0"
	"ga.cp-850" "\0" "CP850" "\0"
	"ga.cp-1252" "\0" "CP1252" "\0"
	"ga.cp-1252@euro" "\0" "CP1252" "\0"
	"ga.utf-8" "\0" "UTF-8" "\0"
	"ga.utf-8@euro" "\0" "UTF-8" "\0"
	"ga_IE" "\0" "ISO-8859-1" "\0"
	"ga_IE.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"ga_IE.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"ga_IE.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"ga_IE@euro" "\0" "ISO-8859-15" "\0"
	"ga_IE.cp-437" "\0" "CP437" "\0"
	"ga_IE.cp-850" "\0" "CP850" "\0"
	"ga_IE.cp-1252" "\0" "CP1252" "\0"
	"ga_IE.cp-1252@euro" "\0" "CP1252" "\0"
	"ga_IE.utf-8" "\0" "UTF-8" "\0"
	"ga_IE.utf-8@euro" "\0" "UTF-8" "\0"
	"gl" "\0" "ISO-8859-1" "\0"
	"gl.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"gl.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"gl.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"gl@euro" "\0" "ISO-8859-15" "\0"
	"gl.cp-437" "\0" "CP437" "\0"
	"gl.cp-850" "\0" "CP850" "\0"
	"gl.cp-1252" "\0" "CP1252" "\0"
	"gl.cp-1252@euro" "\0" "CP1252" "\0"
	"gl.utf-8" "\0" "UTF-8" "\0"
	"gl.utf-8@euro" "\0" "UTF-8" "\0"
	"gl_ES" "\0" "ISO-8859-1" "\0"
	"gl_ES.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"gl_ES.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"gl_ES.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"gl_ES@euro" "\0" "ISO-8859-15" "\0"
	"gl_ES.cp-437" "\0" "CP437" "\0"
	"gl_ES.cp-850" "\0" "CP850" "\0"
	"gl_ES.cp-1252" "\0" "CP1252" "\0"
	"gl_ES.cp-1252@euro" "\0" "CP1252" "\0"
	"gl_ES.utf-8" "\0" "UTF-8" "\0"
	"gl_ES.utf-8@euro" "\0" "UTF-8" "\0"
	"id" "\0" "ISO-8859-1" "\0"
	"id.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"id.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"id.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"id@euro" "\0" "ISO-8859-15" "\0"
	"id.cp-437" "\0" "CP437" "\0"
	"id.cp-850" "\0" "CP850" "\0"
	"id.cp-1252" "\0" "CP1252" "\0"
	"id.cp-1252@euro" "\0" "CP1252" "\0"
	"id.utf-8" "\0" "UTF-8" "\0"
	"id.utf-8@euro" "\0" "UTF-8" "\0"
	"id_ID" "\0" "ISO-8859-1" "\0"
	"id_ID.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"id_ID.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"id_ID.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"id_ID@euro" "\0" "ISO-8859-15" "\0"
	"id_ID.cp-437" "\0" "CP437" "\0"
	"id_ID.cp-850" "\0" "CP850" "\0"
	"id_ID.cp-1252" "\0" "CP1252" "\0"
	"id_ID.cp-1252@euro" "\0" "CP1252" "\0"
	"id_ID.utf-8" "\0" "UTF-8" "\0"
	"id_ID.utf-8@euro" "\0" "UTF-8" "\0"
	"in" "\0" "ISO-8859-1" "\0"
	"in.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"in.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"in.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"in@euro" "\0" "ISO-8859-15" "\0"
	"in.cp-437" "\0" "CP437" "\0"
	"in.cp-850" "\0" "CP850" "\0"
	"in.cp-1252" "\0" "CP1252" "\0"
	"in.cp-1252@euro" "\0" "CP1252" "\0"
	"in.utf-8" "\0" "UTF-8" "\0"
	"in.utf-8@euro" "\0" "UTF-8" "\0"
	"in_ID" "\0" "ISO-8859-1" "\0"
	"in_ID.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"in_ID.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"in_ID.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"in_ID@euro" "\0" "ISO-8859-15" "\0"
	"in_ID.cp-437" "\0" "CP437" "\0"
	"in_ID.cp-850" "\0" "CP850" "\0"
	"in_ID.cp-1252" "\0" "CP1252" "\0"
	"in_ID.cp-1252@euro" "\0" "CP1252" "\0"
	"in_ID.utf-8" "\0" "UTF-8" "\0"
	"in_ID.utf-8@euro" "\0" "UTF-8" "\0"
	"is" "\0" "ISO-8859-1" "\0"
	"is.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"is.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"is.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"is@euro" "\0" "ISO-8859-15" "\0"
	"is.cp-437" "\0" "CP437" "\0"
	"is.cp-850" "\0" "CP850" "\0"
	"is.cp-1252" "\0" "CP1252" "\0"
	"is.cp-1252@euro" "\0" "CP1252" "\0"
	"is.utf-8" "\0" "UTF-8" "\0"
	"is.utf-8@euro" "\0" "UTF-8" "\0"
	"is_IS" "\0" "ISO-8859-1" "\0"
	"is_IS.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"is_IS.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"is_IS.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"is_IS@euro" "\0" "ISO-8859-15" "\0"
	"is_IS.cp-437" "\0" "CP437" "\0"
	"is_IS.cp-850" "\0" "CP850" "\0"
	"is_IS.cp-1252" "\0" "CP1252" "\0"
	"is_IS.cp-1252@euro" "\0" "CP1252" "\0"
	"is_IS.utf-8" "\0" "UTF-8" "\0"
	"is_IS.utf-8@euro" "\0" "UTF-8" "\0"
	"it" "\0" "ISO-8859-1" "\0"
	"it.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"it.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"it.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"it@euro" "\0" "ISO-8859-15" "\0"
	"it.cp-437" "\0" "CP437" "\0"
	"it.cp-850" "\0" "CP850" "\0"
	"it.cp-1252" "\0" "CP1252" "\0"
	"it.cp-1252@euro" "\0" "CP1252" "\0"
	"it.utf-8" "\0" "UTF-8" "\0"
	"it.utf-8@euro" "\0" "UTF-8" "\0"
	"it_CH" "\0" "ISO-8859-1" "\0"
	"it_CH.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"it_CH.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"it_CH.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"it_CH@euro" "\0" "ISO-8859-15" "\0"
	"it_CH.cp-437" "\0" "CP437" "\0"
	"it_CH.cp-850" "\0" "CP850" "\0"
	"it_CH.cp-1252" "\0" "CP1252" "\0"
	"it_CH.cp-1252@euro" "\0" "CP1252" "\0"
	"it_CH.utf-8" "\0" "UTF-8" "\0"
	"it_CH.utf-8@euro" "\0" "UTF-8" "\0"
	"it_IT" "\0" "ISO-8859-1" "\0"
	"it_IT.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"it_IT.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"it_IT.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"it_IT@euro" "\0" "ISO-8859-15" "\0"
	"it_IT.cp-437" "\0" "CP437" "\0"
	"it_IT.cp-850" "\0" "CP850" "\0"
	"it_IT.cp-1252" "\0" "CP1252" "\0"
	"it_IT.cp-1252@euro" "\0" "CP1252" "\0"
	"it_IT.utf-8" "\0" "UTF-8" "\0"
	"it_IT.utf-8@euro" "\0" "UTF-8" "\0"
	"kl" "\0" "ISO-8859-1" "\0"
	"kl.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"kl.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"kl.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"kl@euro" "\0" "ISO-8859-15" "\0"
	"kl.cp-437" "\0" "CP437" "\0"
	"kl.cp-850" "\0" "CP850" "\0"
	"kl.cp-1252" "\0" "CP1252" "\0"
	"kl.cp-1252@euro" "\0" "CP1252" "\0"
	"kl.utf-8" "\0" "UTF-8" "\0"
	"kl.utf-8@euro" "\0" "UTF-8" "\0"
	"kl_GL" "\0" "ISO-8859-1" "\0"
	"kl_GL.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"kl_GL.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"kl_GL.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"kl_GL@euro" "\0" "ISO-8859-15" "\0"
	"kl_GL.cp-437" "\0" "CP437" "\0"
	"kl_GL.cp-850" "\0" "CP850" "\0"
	"kl_GL.cp-1252" "\0" "CP1252" "\0"
	"kl_GL.cp-1252@euro" "\0" "CP1252" "\0"
	"kl_GL.utf-8" "\0" "UTF-8" "\0"
	"kl_GL.utf-8@euro" "\0" "UTF-8" "\0"
	"nl" "\0" "ISO-8859-1" "\0"
	"nl.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"nl.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"nl.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"nl@euro" "\0" "ISO-8859-15" "\0"
	"nl.cp-437" "\0" "CP437" "\0"
	"nl.cp-850" "\0" "CP850" "\0"
	"nl.cp-1252" "\0" "CP1252" "\0"
	"nl.cp-1252@euro" "\0" "CP1252" "\0"
	"nl.utf-8" "\0" "UTF-8" "\0"
	"nl.utf-8@euro" "\0" "UTF-8" "\0"
	"nl_BE" "\0" "ISO-8859-1" "\0"
	"nl_BE.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"nl_BE.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"nl_BE.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"nl_BE@euro" "\0" "ISO-8859-15" "\0"
	"nl_BE.cp-437" "\0" "CP437" "\0"
	"nl_BE.cp-850" "\0" "CP850" "\0"
	"nl_BE.cp-1252" "\0" "CP1252" "\0"
	"nl_BE.cp-1252@euro" "\0" "CP1252" "\0"
	"nl_BE.utf-8" "\0" "UTF-8" "\0"
	"nl_BE.utf-8@euro" "\0" "UTF-8" "\0"
	"nl_NL" "\0" "ISO-8859-1" "\0"
	"nl_NL.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"nl_NL.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"nl_NL.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"nl_NL@euro" "\0" "ISO-8859-15" "\0"
	"nl_NL.cp-437" "\0" "CP437" "\0"
	"nl_NL.cp-850" "\0" "CP850" "\0"
	"nl_NL.cp-1252" "\0" "CP1252" "\0"
	"nl_NL.cp-1252@euro" "\0" "CP1252" "\0"
	"nl_NL.utf-8" "\0" "UTF-8" "\0"
	"nl_NL.utf-8@euro" "\0" "UTF-8" "\0"
	"no" "\0" "ISO-8859-1" "\0"
	"no.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"no.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"no.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"no@euro" "\0" "ISO-8859-15" "\0"
	"no.cp-437" "\0" "CP437" "\0"
	"no.cp-850" "\0" "CP850" "\0"
	"no.cp-1252" "\0" "CP1252" "\0"
	"no.cp-1252@euro" "\0" "CP1252" "\0"
	"no.utf-8" "\0" "UTF-8" "\0"
	"no.utf-8@euro" "\0" "UTF-8" "\0"
	"no_NO" "\0" "ISO-8859-1" "\0"
	"no_NO.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"no_NO.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"no_NO.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"no_NO@euro" "\0" "ISO-8859-15" "\0"
	"no_NO.cp-437" "\0" "CP437" "\0"
	"no_NO.cp-850" "\0" "CP850" "\0"
	"no_NO.cp-1252" "\0" "CP1252" "\0"
	"no_NO.cp-1252@euro" "\0" "CP1252" "\0"
	"no_NO.utf-8" "\0" "UTF-8" "\0"
	"no_NO.utf-8@euro" "\0" "UTF-8" "\0"
	"pt" "\0" "ISO-8859-1" "\0"
	"pt.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"pt.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"pt.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"pt@euro" "\0" "ISO-8859-15" "\0"
	"pt.cp-437" "\0" "CP437" "\0"
	"pt.cp-850" "\0" "CP850" "\0"
	"pt.cp-1252" "\0" "CP1252" "\0"
	"pt.cp-1252@euro" "\0" "CP1252" "\0"
	"pt.utf-8" "\0" "UTF-8" "\0"
	"pt.utf-8@euro" "\0" "UTF-8" "\0"
	"pt_BR" "\0" "ISO-8859-1" "\0"
	"pt_BR.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"pt_BR.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"pt_BR.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"pt_BR@euro" "\0" "ISO-8859-15" "\0"
	"pt_BR.cp-437" "\0" "CP437" "\0"
	"pt_BR.cp-850" "\0" "CP850" "\0"
	"pt_BR.cp-1252" "\0" "CP1252" "\0"
	"pt_BR.cp-1252@euro" "\0" "CP1252" "\0"
	"pt_BR.utf-8" "\0" "UTF-8" "\0"
	"pt_BR.utf-8@euro" "\0" "UTF-8" "\0"
	"pt_PT" "\0" "ISO-8859-1" "\0"
	"pt_PT.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"pt_PT.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"pt_PT.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"pt_PT@euro" "\0" "ISO-8859-15" "\0"
	"pt_PT.cp-437" "\0" "CP437" "\0"
	"pt_PT.cp-850" "\0" "CP850" "\0"
	"pt_PT.cp-1252" "\0" "CP1252" "\0"
	"pt_PT.cp-1252@euro" "\0" "CP1252" "\0"
	"pt_PT.utf-8" "\0" "UTF-8" "\0"
	"pt_PT.utf-8@euro" "\0" "UTF-8" "\0"
	"sv" "\0" "ISO-8859-1" "\0"
	"sv.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"sv.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"sv.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"sv@euro" "\0" "ISO-8859-15" "\0"
	"sv.cp-437" "\0" "CP437" "\0"
	"sv.cp-850" "\0" "CP850" "\0"
	"sv.cp-1252" "\0" "CP1252" "\0"
	"sv.cp-1252@euro" "\0" "CP1252" "\0"
	"sv.utf-8" "\0" "UTF-8" "\0"
	"sv.utf-8@euro" "\0" "UTF-8" "\0"
	"sv_FI" "\0" "ISO-8859-1" "\0"
	"sv_FI.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"sv_FI.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"sv_FI.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"sv_FI@euro" "\0" "ISO-8859-15" "\0"
	"sv_FI.cp-437" "\0" "CP437" "\0"
	"sv_FI.cp-850" "\0" "CP850" "\0"
	"sv_FI.cp-1252" "\0" "CP1252" "\0"
	"sv_FI.cp-1252@euro" "\0" "CP1252" "\0"
	"sv_FI.utf-8" "\0" "UTF-8" "\0"
	"sv_FI.utf-8@euro" "\0" "UTF-8" "\0"
	"sv_SE" "\0" "ISO-8859-1" "\0"
	"sv_SE.iso-8859-1" "\0" "ISO-8859-1" "\0"
	"sv_SE.iso-8859-15" "\0" "ISO-8859-15" "\0"
	"sv_SE.iso-8859-15@euro" "\0" "ISO-8859-15" "\0"
	"sv_SE@euro" "\0" "ISO-8859-15" "\0"
	"sv_SE.cp-437" "\0" "CP437" "\0"
	"sv_SE.cp-850" "\0" "CP850" "\0"
	"sv_SE.cp-1252" "\0" "CP1252" "\0"
	"sv_SE.cp-1252@euro" "\0" "CP1252" "\0"
	"sv_SE.utf-8" "\0" "UTF-8" "\0"
	"sv_SE.utf-8@euro" "\0" "UTF-8" "\0"
	"cs" "\0" "ISO-8859-2" "\0"
	"cs.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"cs.cp-852" "\0" "CP852" "\0"
	"cs.cp-1250" "\0" "CP1250" "\0"
	"cs.utf-8" "\0" "UTF-8" "\0"
	"cs_CZ" "\0" "ISO-8859-2" "\0"
	"cs_CZ.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"cs_CZ.cp-852" "\0" "CP852" "\0"
	"cs_CZ.cp-1250" "\0" "CP1250" "\0"
	"cs_CZ.utf-8" "\0" "UTF-8" "\0"
	"hr" "\0" "ISO-8859-2" "\0"
	"hr.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"hr.cp-852" "\0" "CP852" "\0"
	"hr.cp-1250" "\0" "CP1250" "\0"
	"hr.utf-8" "\0" "UTF-8" "\0"
	"hr_HR" "\0" "ISO-8859-2" "\0"
	"hr_HR.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"hr_HR.cp-852" "\0" "CP852" "\0"
	"hr_HR.cp-1250" "\0" "CP1250" "\0"
	"hr_HR.utf-8" "\0" "UTF-8" "\0"
	"hu" "\0" "ISO-8859-2" "\0"
	"hu.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"hu.cp-852" "\0" "CP852" "\0"
	"hu.cp-1250" "\0" "CP1250" "\0"
	"hu.utf-8" "\0" "UTF-8" "\0"
	"hu_HU" "\0" "ISO-8859-2" "\0"
	"hu_HU.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"hu_HU.cp-852" "\0" "CP852" "\0"
	"hu_HU.cp-1250" "\0" "CP1250" "\0"
	"hu_HU.utf-8" "\0" "UTF-8" "\0"
	"pl" "\0" "ISO-8859-2" "\0"
	"pl.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"pl.cp-852" "\0" "CP852" "\0"
	"pl.cp-1250" "\0" "CP1250" "\0"
	"pl.utf-8" "\0" "UTF-8" "\0"
	"pl_PL" "\0" "ISO-8859-2" "\0"
	"pl_PL.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"pl_PL.cp-852" "\0" "CP852" "\0"
	"pl_PL.cp-1250" "\0" "CP1250" "\0"
	"pl_PL.utf-8" "\0" "UTF-8" "\0"
	"ro" "\0" "ISO-8859-2" "\0"
	"ro.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"ro.cp-852" "\0" "CP852" "\0"
	"ro.cp-1250" "\0" "CP1250" "\0"
	"ro.utf-8" "\0" "UTF-8" "\0"
	"ro_RO" "\0" "ISO-8859-2" "\0"
	"ro_RO.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"ro_RO.cp-852" "\0" "CP852" "\0"
	"ro_RO.cp-1250" "\0" "CP1250" "\0"
	"ro_RO.utf-8" "\0" "UTF-8" "\0"
	"sk" "\0" "ISO-8859-2" "\0"
	"sk.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"sk.cp-852" "\0" "CP852" "\0"
	"sk.cp-1250" "\0" "CP1250" "\0"
	"sk.utf-8" "\0" "UTF-8" "\0"
	"sk_SK" "\0" "ISO-8859-2" "\0"
	"sk_SK.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"sk_SK.cp-852" "\0" "CP852" "\0"
	"sk_SK.cp-1250" "\0" "CP1250" "\0"
	"sk_SK.utf-8" "\0" "UTF-8" "\0"
	"sl" "\0" "ISO-8859-2" "\0"
	"sl.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"sl.cp-852" "\0" "CP852" "\0"
	"sl.cp-1250" "\0" "CP1250" "\0"
	"sl.utf-8" "\0" "UTF-8" "\0"
	"sl_SI" "\0" "ISO-8859-2" "\0"
	"sl_SI.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"sl_SI.cp-852" "\0" "CP852" "\0"
	"sl_SI.cp-1250" "\0" "CP1250" "\0"
	"sl_SI.utf-8" "\0" "UTF-8" "\0"
	"sr" "\0" "ISO-8859-2" "\0"
	"sr.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"sr.cp-852" "\0" "CP852" "\0"
	"sr.cp-1250" "\0" "CP1250" "\0"
	"sr.utf-8" "\0" "UTF-8" "\0"
	"sr_CS" "\0" "ISO-8859-2" "\0"
	"sr_CS.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"sr_CS.cp-852" "\0" "CP852" "\0"
	"sr_CS.cp-1250" "\0" "CP1250" "\0"
	"sr_CS.utf-8" "\0" "UTF-8" "\0"
	"sr_YU" "\0" "ISO-8859-2" "\0"
	"sr_YU.iso-8859-2" "\0" "ISO-8859-2" "\0"
	"sr_YU.cp-852" "\0" "CP852" "\0"
	"sr_YU.cp-1250" "\0" "CP1250" "\0"
	"sr_YU.utf-8" "\0" "UTF-8" "\0"
	"mk" "\0" "ISO-8859-5" "\0"
	"mk.iso-8859-5" "\0" "ISO-8859-5" "\0"
	"mk.koi8-r" "\0" "KOI8-R" "\0"
	"mk.cp-866" "\0" "CP866" "\0"
	"mk.cp-1251" "\0" "CP1251" "\0"
	"mk.utf-8" "\0" "UTF-8" "\0"
	"mk_MK" "\0" "ISO-8859-5" "\0"
	"mk_MK.iso-8859-5" "\0" "ISO-8859-5" "\0"
	"mk_MK.koi8-r" "\0" "KOI8-R" "\0"
	"mk_MK.cp-866" "\0" "CP866" "\0"
	"mk_MK.cp-1251" "\0" "CP1251" "\0"
	"mk_MK.utf-8" "\0" "UTF-8" "\0"
	"ru" "\0" "ISO-8859-5" "\0"
	"ru.iso-8859-5" "\0" "ISO-8859-5" "\0"
	"ru.koi8-r" "\0" "KOI8-R" "\0"
	"ru.cp-866" "\0" "CP866" "\0"
	"ru.cp-1251" "\0" "CP1251" "\0"
	"ru.utf-8" "\0" "UTF-8" "\0"
	"ru_RU" "\0" "ISO-8859-5" "\0"
	"ru_RU.iso-8859-5" "\0" "ISO-8859-5" "\0"
	"ru_RU.koi8-r" "\0" "KOI8-R" "\0"
	"ru_RU.cp-866" "\0" "CP866" "\0"
	"ru_RU.cp-1251" "\0" "CP1251" "\0"
	"ru_RU.utf-8" "\0" "UTF-8" "\0"
	"ar" "\0" "ISO-8859-6" "\0"
	"ar.iso-8859-6" "\0" "ISO-8859-6" "\0"
	"ar.cp-864" "\0" "CP864" "\0"
	"ar.cp-1256" "\0" "CP1256" "\0"
	"ar.utf-8" "\0" "UTF-8" "\0"
	"ar_SA" "\0" "ISO-8859-6" "\0"
	"ar_SA.iso-8859-6" "\0" "ISO-8859-6" "\0"
	"ar_SA.cp-864" "\0" "CP864" "\0"
	"ar_SA.cp-1256" "\0" "CP1256" "\0"
	"ar_SA.utf-8" "\0" "UTF-8" "\0"
	"el" "\0" "ISO-8859-7" "\0"
	"el.iso-8859-7" "\0" "ISO-8859-7" "\0"
	"el.cp-869" "\0" "CP869" "\0"
	"el.cp-1253" "\0" "CP1253" "\0"
	"el.cp-1253@euro" "\0" "CP1253" "\0"
	"el.utf-8" "\0" "UTF-8" "\0"
	"el.utf-8@euro" "\0" "UTF-8" "\0"
	"el_GR" "\0" "ISO-8859-7" "\0"
	"el_GR.iso-8859-7" "\0" "ISO-8859-7" "\0"
	"el_GR.cp-869" "\0" "CP869" "\0"
	"el_GR.cp-1253" "\0" "CP1253" "\0"
	"el_GR.cp-1253@euro" "\0" "CP1253" "\0"
	"el_GR.utf-8" "\0" "UTF-8" "\0"
	"el_GR.utf-8@euro" "\0" "UTF-8" "\0"
	"gr" "\0" "ISO-8859-7" "\0"
	"gr.iso-8859-7" "\0" "ISO-8859-7" "\0"
	"gr.cp-869" "\0" "CP869" "\0"
	"gr.cp-1253" "\0" "CP1253" "\0"
	"gr.cp-1253@euro" "\0" "CP1253" "\0"
	"gr.utf-8" "\0" "UTF-8" "\0"
	"gr.utf-8@euro" "\0" "UTF-8" "\0"
	"gr_GR" "\0" "ISO-8859-7" "\0"
	"gr_GR.iso-8859-7" "\0" "ISO-8859-7" "\0"
	"gr_GR.cp-869" "\0" "CP869" "\0"
	"gr_GR.cp-1253" "\0" "CP1253" "\0"
	"gr_GR.cp-1253@euro" "\0" "CP1253" "\0"
	"gr_GR.utf-8" "\0" "UTF-8" "\0"
	"gr_GR.utf-8@euro" "\0" "UTF-8" "\0"
	"he" "\0" "ISO-8859-8" "\0"
	"he.iso-8859-8" "\0" "ISO-8859-8" "\0"
	"he.cp-862" "\0" "CP862" "\0"
	"he.cp-1255" "\0" "CP1255" "\0"
	"he.utf-8" "\0" "UTF-8" "\0"
	"he_IL" "\0" "ISO-8859-8" "\0"
	"he_IL.iso-8859-8" "\0" "ISO-8859-8" "\0"
	"he_IL.cp-862" "\0" "CP862" "\0"
	"he_IL.cp-1255" "\0" "CP1255" "\0"
	"he_IL.utf-8" "\0" "UTF-8" "\0"
	"iw" "\0" "ISO-8859-8" "\0"
	"iw.iso-8859-8" "\0" "ISO-8859-8" "\0"
	"iw.cp-862" "\0" "CP862" "\0"
	"iw.cp-1255" "\0" "CP1255" "\0"
	"iw.utf-8" "\0" "UTF-8" "\0"
	"iw_IL" "\0" "ISO-8859-8" "\0"
	"iw_IL.iso-8859-8" "\0" "ISO-8859-8" "\0"
	"iw_IL.cp-862" "\0" "CP862" "\0"
	"iw_IL.cp-1255" "\0" "CP1255" "\0"
	"iw_IL.utf-8" "\0" "UTF-8" "\0"
	"tr" "\0" "ISO-8859-9" "\0"
	"tr.iso-8859-9" "\0" "ISO-8859-9" "\0"
	"tr.cp-857" "\0" "CP857" "\0"
	"tr.cp-1254" "\0" "CP1254" "\0"
	"tr.utf-8" "\0" "UTF-8" "\0"
	"tr_TR" "\0" "ISO-8859-9" "\0"
	"tr_TR.iso-8859-9" "\0" "ISO-8859-9" "\0"
	"tr_TR.cp-857" "\0" "CP857" "\0"
	"tr_TR.cp-1254" "\0" "CP1254" "\0"
	"tr_TR.utf-8" "\0" "UTF-8" "\0"
	"lt" "\0" "ISO-8859-13" "\0"
	"lt_LT" "\0" "ISO-8859-13" "\0"
	"lv" "\0" "ISO-8859-13" "\0"
	"lv_LV" "\0" "ISO-8859-13" "\0"
	"ru_UA" "\0" "KOI8-U" "\0"
	"uk" "\0" "KOI8-U" "\0"
	"uk_UA" "\0" "KOI8-U" "\0"
	"zh" "\0" "GB2312" "\0"
	"zh_CN" "\0" "GB2312" "\0"
	"ja" "\0" "EUC-JP" "\0"
	"ja_JP" "\0" "EUC-JP" "\0"
	"ja_JP.EUC" "\0" "EUC-JP" "\0"
	"ko" "\0" "EUC-KR" "\0"
	"ko_KR" "\0" "EUC-KR" "\0"
	"th" "\0" "TIS-620" "\0"
	"th_TH" "\0" "TIS-620" "\0"
	"fa.utf-8" "\0" "UTF-8" "\0"
	"fa_IR.utf-8" "\0" "UTF-8" "\0";

#endif

#endif

      charset_aliases = cp;
    }

  return cp;
}

/* Determine the current locale's character encoding, and canonicalize it
   into one of the canonical names listed in config.charset.
   The result must not be freed; it is statically allocated.
   If the canonical name cannot be determined, the result is a non-canonical
   name.  */

#ifdef STATIC
STATIC
#endif
const char *
locale_charset (void)
{
  const char *codeset;
  const char *aliases;

#if !(defined WIN32_NATIVE || defined OS2)

# if HAVE_LANGINFO_CODESET

  /* Most systems support nl_langinfo (CODESET) nowadays.  */
  codeset = nl_langinfo (CODESET);

#  ifdef __CYGWIN__
  /* Cygwin < 1.7 does not have locales.  nl_langinfo (CODESET) always
     returns "US-ASCII".  Return the suffix of the locale name from the
     environment variables (if present) or the codepage as a number.  */
  if (codeset != NULL && strcmp (codeset, "US-ASCII") == 0)
    {
      const char *locale;
      static char buf[2 + 10 + 1];

      locale = getenv ("LC_ALL");
      if (locale == NULL || locale[0] == '\0')
        {
          locale = getenv ("LC_CTYPE");
          if (locale == NULL || locale[0] == '\0')
            locale = getenv ("LANG");
        }
      if (locale != NULL && locale[0] != '\0')
        {
          /* If the locale name contains an encoding after the dot, return
             it.  */
          const char *dot = strchr (locale, '.');

          if (dot != NULL)
            {
              const char *modifier;

              dot++;
              /* Look for the possible @... trailer and remove it, if any.  */
              modifier = strchr (dot, '@');
              if (modifier == NULL)
                return dot;
              if (modifier - dot < sizeof (buf))
                {
                  memcpy (buf, dot, modifier - dot);
                  buf [modifier - dot] = '\0';
                  return buf;
                }
            }
        }

      /* Woe32 has a function returning the locale's codepage as a number:
         GetACP().  This encoding is used by Cygwin, unless the user has set
         the environment variable CYGWIN=codepage:oem (which very few people
         do).
         Output directed to console windows needs to be converted (to
         GetOEMCP() if the console is using a raster font, or to
         GetConsoleOutputCP() if it is using a TrueType font).  Cygwin does
         this conversion transparently (see winsup/cygwin/fhandler_console.cc),
         converting to GetConsoleOutputCP().  This leads to correct results,
         except when SetConsoleOutputCP has been called and a raster font is
         in use.  */
      sprintf (buf, "CP%u", GetACP ());
      codeset = buf;
    }
#  endif

# else

  /* On old systems which lack it, use setlocale or getenv.  */
  const char *locale = NULL;

  /* But most old systems don't have a complete set of locales.  Some
     (like SunOS 4 or DJGPP) have only the C locale.  Therefore we don't
     use setlocale here; it would return "C" when it doesn't support the
     locale name the user has set.  */
#  if 0
  locale = setlocale (LC_CTYPE, NULL);
#  endif
  if (locale == NULL || locale[0] == '\0')
    {
      locale = getenv ("LC_ALL");
      if (locale == NULL || locale[0] == '\0')
        {
          locale = getenv ("LC_CTYPE");
          if (locale == NULL || locale[0] == '\0')
            locale = getenv ("LANG");
        }
    }

  /* On some old systems, one used to set locale = "iso8859_1". On others,
     you set it to "language_COUNTRY.charset". In any case, we resolve it
     through the charset.alias file.  */
  codeset = locale;

# endif

#elif defined WIN32_NATIVE

  static char buf[2 + 10 + 1];

  /* Woe32 has a function returning the locale's codepage as a number:
     GetACP().
     When the output goes to a console window, it needs to be provided in
     GetOEMCP() encoding if the console is using a raster font, or in
     GetConsoleOutputCP() encoding if it is using a TrueType font.
     But in GUI programs and for output sent to files and pipes, GetACP()
     encoding is the best bet.  */
  sprintf (buf, "CP%u", GetACP ());
  codeset = buf;

#elif defined OS2

  const char *locale;
  static char buf[2 + 10 + 1];
  ULONG cp[3];
  ULONG cplen;

  /* Allow user to override the codeset, as set in the operating system,
     with standard language environment variables.  */
  locale = getenv ("LC_ALL");
  if (locale == NULL || locale[0] == '\0')
    {
      locale = getenv ("LC_CTYPE");
      if (locale == NULL || locale[0] == '\0')
        locale = getenv ("LANG");
    }
  if (locale != NULL && locale[0] != '\0')
    {
      /* If the locale name contains an encoding after the dot, return it.  */
      const char *dot = strchr (locale, '.');

      if (dot != NULL)
        {
          const char *modifier;

          dot++;
          /* Look for the possible @... trailer and remove it, if any.  */
          modifier = strchr (dot, '@');
          if (modifier == NULL)
            return dot;
          if (modifier - dot < sizeof (buf))
            {
              memcpy (buf, dot, modifier - dot);
              buf [modifier - dot] = '\0';
              return buf;
            }
        }

      /* Resolve through the charset.alias file.  */
      codeset = locale;
    }
  else
    {
      /* OS/2 has a function returning the locale's codepage as a number.  */
      if (DosQueryCp (sizeof (cp), cp, &cplen))
        codeset = "";
      else
        {
          sprintf (buf, "CP%u", cp[0]);
          codeset = buf;
        }
    }

#endif

  if (codeset == NULL)
    /* The canonical name cannot be determined.  */
    codeset = "";

  /* Resolve alias. */
  for (aliases = get_charset_aliases ();
       *aliases != '\0';
       aliases += strlen (aliases) + 1, aliases += strlen (aliases) + 1)

#ifdef _WIN32
	if (stricmp (codeset, aliases) == 0 || (aliases[0] == '*' && aliases[1] == '\0'))
#else
	  if (strcasecmp (codeset, aliases) == 0 || (aliases[0] == '*' && aliases[1] == '\0'))
#endif
      {
        codeset = aliases + strlen (aliases) + 1;
        break;
      }

  /* Don't return an empty string.  GNU libc and GNU libiconv interpret
     the empty string as denoting "the locale's character encoding",
     thus GNU libiconv would call this function a second time.  */
  if (codeset[0] == '\0')
    codeset = "ASCII";

  return codeset;
}

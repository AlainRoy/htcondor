#ifndef _TYPES_H_
#define _TYPES_H_

#include "condor_common.h"    /* for <stdio.h> */

//---------------------------------------------------------------------------
#ifndef __cplusplus
typedef int bool;
#define true  1
#define false 0
#endif

namespace dagman {

///
typedef int JobID_t;

//---------------------------------------------------------------------------
/** Condor uses three integers to identify jobs. This structure 
    will be used to store those three numbers.  
*/
class CondorID {
  public:
    ///
    CondorID () : _cluster(-1), _proc(-1), _subproc(-1) {}

    ///
    CondorID (int cluster, int proc, int subproc):
        _cluster(cluster), _proc(proc), _subproc(subproc) {}

    ///
    inline void Set (int cluster, int proc, int subproc) {
        _cluster = cluster;
        _proc    = proc;
        _subproc = subproc;
    }

    /** Compare this condorID's with another
        @return zero if they match
    */
    int Compare (const CondorID condorID) const;

    ///
    inline bool operator == (const CondorID condorID) const {
        return Compare (condorID) == 0;
    }

    ///
    inline void Print (FILE * file = stdout) const {
        fprintf (file, "(%d.%d.%d)", _cluster, _proc, _subproc);
    }

    /** */ int _cluster;
    /** */ int _proc;
    /** */ int _subproc;
};


//-----------------------------------------------------------------------------
class string {
  friend ostream & operator << (ostream & out, const string & s);
  friend string operator + (const string & s1, const string & s2);
  public:
    string () : _str(NULL) {}
    string (const string & s);
    string (const char   * s);
    string (const char c);
    string (const int  i);
    const string & operator = (const string & s);
    const char   * operator = (const char   * s);
    const string & operator += (const string & s);
    bool           operator == (const string & s) const;
    bool           operator != (const string & s) const;

    inline const char * str() const { return _str; }

    ~string () { delete [] _str; }
  private:
    string (const string & s1, const string & s2);
    char * _str;
};


ostream & operator << (ostream & out, const string & s);
string operator + (const string & s1,
                   const string & s2);

} // namespace dagman

#endif /* #ifndef _TYPES_H_ */

#ifndef _ClassAdCollection_H
#define _ClassAdCollection_H

//--------------------------------------------------------------------------

#include "condor_classad.h"
#include "Set.h"
#include "HashTable.h"
#include "MyString.h"
#include "log.h"
#include "classad_hashtable.h"
#include "log_transaction.h"
#include "classad_collection_types.h"

//--------------------------------------------------------------------------

typedef HashTable<int,BaseCollection*> CollectionHashTable;
typedef HashTable <HashKey, ClassAd *> ClassAdHashTable;
int partitionHashFcn( const MyString &, int );

///--------------------------------------------------------------------------

//@author Adiel Yoaz
///@include: classad_collection_types.h

/** This is the repository main class. Using the methods of this class
    the user can create and delete class-ads, change their attributes,
    control transactions, create logical collections of class-ads, and
    iterate through the class-ads and the collections. Note that only 
    the operations relating to class-ads are logged. Collection operations
    are not logged, therefore collections are not persistent.

    @author Adiel Yoaz
*/

class ClassAdCollection {

friend class CollChildIterator;
friend class CollContentIterator;

friend class LogCollNewClassAd;
friend class LogCollDestroyClassAd;
friend class LogCollUpdateClassAd;

public:

  //------------------------------------------------------------------------
  /**@name Constructor and Destructor
  */
  //@{

  /** Constructor (initialization). It reads the log file and initializes
      the class-ads (that are read from the log file) in memory.
    @param filename the name of the log file. If a NULL value is passed, no 
		log file will be used.
    @param rank The rank expression for the collection
    @return nothing
  */
  ClassAdCollection(const char* filename, const MyString& rank);

  /** Constructor (initialization). It reads the log file and initializes
      the class-ads (that are read from the log file) in memory.
    @param filename the name of the log file. If a NULL value is passed, no 
		log file will be used.
    @param rank a pointer to the expression tree which will be used to rank 
		the collection
    @return nothing
  */
  ClassAdCollection(const char* filename=NULL, ExprTree* rank=NULL);

  /** Destructor - frees the memory used by the collections
    @return nothing
  */
  ~ClassAdCollection();

  //@}
  //------------------------------------------------------------------------
  /**@name Transaction control methods
  	Note that when no transaction is active, all persistent operations 
	(creating and deleting class-ads, changing attributes), are logged 
	immediatly. When a transaction is active (after BeginTransaction has been 
	called), the changes are saved only when the transaction is commited.
  */
  //@{

  /** Begin a transaction
    @return nothing
  */
  void BeginTransaction();

  /** Commit a transaction
    @return nothing
  */
  void CommitTransaction();

  /** Abort a transaction
    @return nothing
  */
  void AbortTransaction();

  /** Check if a transaction is in progress
    @returns true if a transaction is active
  */
  bool IsActiveTransaction();

  /** Truncate the log file by creating a new "checkpoint" of the repository
    @return nothing
  */
  void TruncLog();

  //@}
  //------------------------------------------------------------------------
  /**@name Method to control the class-ads in the repository
  */
  //@{

  /** Insert a new class-ad with the specified key.  The supplied class-ad will 
    	be inserted directly into the collection hierarchy, overwriting any 
		previous class-ad with the same key.
      @param key The class-ad's key.
      @param ad The class-ad to put into the repository. The caller should not
        access this pointer to update the class-ad after calling this method.
      @return nothing
  */
  void NewClassAd(const char* key, ClassAd* ad);

  /** Destroy the class-ad with the specified key.
      @param key The class-ad's key.
      @return nothing
  */
  void DestroyClassAd(const char* key);

  /** Update a class-ad in the collection hierarchy.
      @param key The class-ad's key.
      @param ad The ad that represents the update. All the attributes from this 
	  	ad will be inserted into the class-ad with the specified key. This 
		pointer should not be accessed by the user after calling this method.
      @return nothing.
  */
  void UpdateClassAd(const char* key, ClassAd* ad);

  /** Get a class-ad from the repository.
      Note that the class-ad returned cannot be modified directly.
      @param key The class-ad's key.
      @return a pointer to a new class-ad which is the value of the
        original ad including the changes in the current transaction,
        or null if a class-ad with the specified key doesn't exist.
      @return true on success, false otherwise.
  */
  ClassAd* LookupClassAd(const char* key);
  
  /** Lookup a class-ad value including updates made to it in the current
      (active) transaction (if there are any).
      @param key the key with which the class-ad was inserted into the 
	  	repository.
      @return a pointer to a new class-ad which is the value of the
        original ad including the changes in the current transaction,
        or null if a class-ad with the specified key doesn't exist.
  */
  ClassAd* LookupInTransaction(const char *key);
  
  //@}
  //------------------------------------------------------------------------
  /**@name Collection Operations
  * Note: these operations are not persistent - not logged.
  */
  //@{

  /** Create a constraint collection, as a child of another collection.
      A constraint collection always contains the subset of ads from the parent,
	  which match the constraint.
      @param ParentCoID The ID of the parent collection.
      @param Rank The rank expression in string format. Determines how the ads 
	  	will be ordered in the collection.
      @param Constraint sets the constraint expression for this collection.
      @return the ID of the new collection, or -1 in case of failure.
  */
  int CreateConstraintCollection(int ParentCoID, const MyString& Rank, 
		  const MyString& Constraint);

  /** Create a constraint collection, as a child of another collection.
      A constraint collection always contains the subset of ads from the parent,
	  which match the constraint.
      @param ParentCoID The ID of the parent collection.
      @param Rank The rank expression as an expression tree. Determines how the 
	  	ads will be ordered in the collection.
      @param Constraint sets the constraint expression for this collection.
      @return the ID of the new collection, or -1 in case of failure.
  */
  int CreateConstraintCollection(int ParentCoID, ExprTree *Rank, 
		  ExprTree *Constraint);

  /** Create a partition parent collection, as a child of another collection.
      A partiton collection defines a partition based on a set of attributes. 
	  For each distinct set of values (corresponding to these attributes), a new
      child partition collection will be created, which will contain all the 
	  class-ads from the parent collection that have these values. The partition
	  parent collection itself doesn't hold any class-ads, only its children do 
	  (the iteration methods for getting child collections can be used to 
	  retrieve them).
      @param ParentCoID The ID of the parent collection.
      @param Rank The rank expression in string format. Determines how the ads 
	  	will be ordered in the child collections.
      @param AttrList The set of attribute names used to define the partition.
      @return the ID of the new collection, or -1 in case of failure.
  */
  int CreatePartition(int ParentCoID, const MyString& Rank,StringSet& AttrList);

  /** Create a partition parent collection, as a child of another collection.
      A partiton collection defines a partition based on a set of attributes. 
	  For each distinct set of values (corresponding to these attributes), a new
      child partition collection will be created, which will contain all the 
	  class-ads from the parent collection that have these values. The partition
	  parent collection itself doesn't hold any class-ads, only its children do 
	  (the iteration methods for getting child collections can be used to 
	  retrieve them).
      @param ParentCoID The ID of the parent collection.
      @param Rank The rank expression as an expression tree. Determines how the 
	  	ads will be ordered in the child collections.
      @param AttrList The set of attribute names used to define the partition.
      @return the ID of the new collection, or -1 in case of failure.
  */
  int CreatePartition(int ParentCoID, ExprTree* Rank, StringSet& AttrList);

  /** Find the partition of a class-ad. The method returns the ID of the child 
    	partition that would contain the specified class-ad.
      @param ParenCoID the ID of the partition parent collection.
      @param representative a pointer to the class-ad that will be checked.
      @return the ID of the child partition collection that would contain the 
	  	ad.
  */
  int FindPartition(int ParentCoID, ClassAd *representative );

  /** Deletes a collection and all of its descendants from the collection tree.
      @param CoID The ID of the collection to be deleted.
      @return true on success, false otherwise.
  */
  bool DeleteCollection(int CoID);

  /** Get the size of a collection.
      @param CoID The ID of the collection (default is the root collection)
      @return the size of the collection, or -1 if it doesn't exist
  */
  int Size(int CoID=0);

  /** Get the rank value of a class-ad in a specified collection. This method 
    	can also be used to check if the class-ad is a member of the collection.
      @param key the key of the class-ad.
      @param rank_value the numeric rank of the class-ad in the collection.
      @param CoID The ID of the collection (default is the root collection).
      @return true if the class-ad with the specified key is in the collection, 
	  	false otherwise.
  */
  bool GetRankValue(char* key, float& rank_value, int CoID=0);

  //@}

  //------------------------------------------------------------------------
  //@name Iteration methods
  //@{

  bool InitializeIterator( int CoID, CollContentIterator& i );
  bool InitializeIterator( int CoID, CollChildIterator& i );

  /** Start iterations on all the class-ad keys in a collection.
      @param CoID the ID of the collection. The default is the root collection.
      @return true if the specified collection exists, false otherwise.
  */
  bool StartIterateClassAds( int CoID=0 );

  /** Get the next class-ad key in the repository.
      @param key a pointer whick will be assigned the next key value.
      @param CoID the ID of the collection. The default is the root collection.
      @return true on success, false if there are no more keys.
  */
  void StartIterateAllCollections();

  /** Get the next collection ID
      @param CoID The ID of the next collection (output parameter).
      @return true on success, false otherwise.
  */
  bool IterateAllCollections(int& CoID);
  bool IterateClassAds( char* key, int CoID=0 );

  /** Start iterations on child collections of a specified collection.
      @param ParentCoID The ID of the parent of the collections to be iterated 
	  	on.
      @return true on success, false otherwise.
  */
  bool StartIterateChildCollections(int ParentCoID=0);

  /** Get the next child of the specified parent collection.
      @param CoID The ID of the next collection (output parameter).
      @param ParentCoID The ID of the parent of the collections to be iterated 
	  	on.
      @return true on success, false otherwise.
  */
  bool IterateChildCollections(int& CoID, int ParentCoID=0);

  //@}
  //------------------------------------------------------------------------

  /**@name Misc methods
  */
  //@{

  /** Find out a collection's type (explicit, constraint, ...).
      @return the type of the specified collection: 
	  	0=root, 1=constraint, 2=partition parent, 3=partition child.
  */
  int GetCollectionType(int CoID);

  /** Prints the whole repository (for debugging purposes).
      @return nothing.
  */
  void Print();

  /** Prints a single collection in the repository (for debugging purposes).
      @return nothing.
  */
  void Print(int CoID);

  /** Prints all of the class-ads (with their content).
      @return nothing.
  */
  void PrintAllAds();

  static int HashFunc(const int& Key, int TableSize) { return (Key % TableSize); }

  //@}
  //------------------------------------------------------------------------

private:

  bool IterateClassAds( RankedClassAd &Ranked, int CoID );
  //------------------------------------------------------------------------
  // Data Members
  //------------------------------------------------------------------------
  CollectionHashTable Collections;
  int LastCoID;
  //------------------------------------------------------------------------
  // Methods that are used internally
  //------------------------------------------------------------------------
  bool AddClassAd(int CoID, const MyString& OID);
  bool AddClassAd(int CoID, const MyString& OID, ClassAd* ad);
  bool RemoveClassAd(int CoID, const MyString& OID);
  bool ChangeClassAd(const MyString& OID, ClassAd* ad);
  bool RemoveCollection(int CoID, BaseCollection* Coll);
  bool TraverseTree(int CoID, 
		  bool (ClassAdCollection::*Func)(int,BaseCollection*));
  static bool EqualSets(StringSet& S1, StringSet& S2);
  bool CheckClassAd(int CoID, BaseCollection* Coll, const MyString& OID, ClassAd* Ad);
  BaseCollection* GetCollection(int CoID);

  void PlayDestroyClassAd(const char* key);
  void PlayNewClassAd(const char* key, ClassAd* ad);
  void PlayUpdateClassAd(const char* key, ClassAd* ad);

  void AppendLog(LogRecord *log);
  
    ClassAdHashTable table;  // Hash table of class ads in memory

    void ReadLog(const char* filename);
    LogRecord* ReadLogEntry(FILE* fp);
    LogRecord* InstantiateLogEntry(FILE* fp, int type);

    void    LogState(FILE *fp);
    char    log_filename[_POSIX_PATH_MAX];
    FILE    *log_fp;
    Source  src;
    Sink    snk;
    bool    EmptyTransaction;
    Transaction *active_transaction;

};

#endif

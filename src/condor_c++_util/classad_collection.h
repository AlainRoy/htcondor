#ifndef _ClassAdCollection_H
#define _ClassAdCollection_H

//--------------------------------------------------------------------------

#include "condor_classad.h"
#include "Set.h"
#include "HashTable.h"
#include "MyString.h"
#include "classad_log.h"

#include "classad_collection_types.h"

//--------------------------------------------------------------------------

typedef HashTable<int,BaseCollection*> CollectionHashTable;
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

class ClassAdCollection : private ClassAdLog {
friend CollChildIterator;
friend CollContentIterator;
public:

  //------------------------------------------------------------------------
  /**@name Constructor and Destructor
  */
  //@{

  /** Constructor (initialization). No logging is done if the log
      filename is not given using this constructor. We start with an
      empty repository.
    @return nothing
  */
  ClassAdCollection();

  /** Constructor (initialization). It reads the log file and initializes
      the class-ads (that are read from the log file) in memory.
    @param filename the name of the log file.
    @return nothing
  */
  ClassAdCollection(const char* filename);

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
  void BeginTransaction() { ClassAdLog::BeginTransaction(); }

  /** Commit a transaction
    @return nothing
  */
  void CommitTransaction() { ClassAdLog::CommitTransaction(); }

  /** Abort a transaction
    @return nothing
  */
  void AbortTransaction() { ClassAdLog::AbortTransaction(); }

  /** Lookup an attribute's value in the current transaction. 
      @param key the key with which the class-ad was inserted into the 
	  	repository.
      @param name the name of the attribute.
      @param expr the attribute expression (output parameter).
      @return true on success, false otherwise.
  */
  bool LookupInTransaction(const char *key, const char *name, ExprTree *&val) { 
	 return (ClassAdLog::LookupInTransaction(key,name,val)==1); 
  }
  
  /** Truncate the log file by creating a new "checkpoint" of the repository
    @return nothing
  */
  void TruncLog() { ClassAdLog::TruncLog(); }

  //@}
  //------------------------------------------------------------------------
  /**@name Method to control the class-ads in the repository
  */
  //@{

  /** Insert a new empty class-ad with the specified key.
      @param key The class-ad's key.
      @return true on success, false otherwise.
  */
  bool NewClassAd(const char* key);

  /** Insert a new class-ad with the specified key.
      The new class-ad will be a copy of the ad supplied.
      @param key The class-ad's key.
      @param ad The class-ad to copy into the repository.
      @return true on success, false otherwise.
  */
  bool NewClassAd(const char* key, ClassAd* ad);

  /** Destroy the class-ad with the specified key.
      @param key The class-ad's key.
      @return true on success, false otherwise.
  */
  bool DestroyClassAd(const char* key);

  /** Set an attribute in a class-ad.
      @param key The class-ad's key.
      @param name the name of the attribute.
      @param expr the attribute expression
      @return true on success, false otherwise.
  */
  bool SetAttribute(const char* key, const char* name, ExprTree* expr);
  bool SetAttribute(const char* key, const char* name, const char *expr, 
		  	int len=-1);

  /** Delete an attribute in a class-ad.
      @param key The class-ad's key.
      @param name the name of the attribute.
      @return true on success, false otherwise.
  */
  bool DeleteAttribute(const char* key, const char* name);

  /** Get a class-ad from the repository.
      Note that the class-ad returned cannot be modified directly.
      @param key The class-ad's key.
      @param Ad A pointer to the class-ad (output parameter).
      @return true on success, false otherwise.
  */
  bool LookupClassAd(const char* key, ClassAd*& Ad) 
  { 
	  return (table.lookup(HashKey(key), Ad)==0); 
  }
  
  //@}
  //------------------------------------------------------------------------
  /**@name Collection Operations
  * Note: these operations are not persistent - not logged.
  */
  //@{

  /** Create an explicit collection, as a child of another collection.
      An explicit collection can include any subset of ads which are in its 
	  parent.  The user can actively include and exclude ads from this 
	  collection.
      @param ParentCoID The ID of the parent collection.
      @param Rank The rank expression. Determines how the ads will be ordered 
	  	in the collection.
      @param FullFlag The flag which indicates automatic insertion of class-ads 
	  	from the parent.
      @return the ID of the new collection, or -1 in case of failure.
  */
  int CreateExplicitCollection(int ParentCoID, const MyString& Rank, 
		  bool FullFlag=false);
  /// Same as above, but pass in a classad expression for the rank
  int CreateExplicitCollection(int ParentCoID, ExprTree *Rank, 
		  bool FullFlag=false);

  /** Create a constraint collection, as a child of another collection.
      A constraint collection always contains the subset of ads from the parent,
	  which match the constraint.
      @param ParentCoID The ID of the parent collection.
      @param Rank The rank expression. Determines how the ads will be ordered 
	  	in the collection.
      @param Constraint sets the constraint expression for this collection.
      @return the ID of the new collection, or -1 in case of failure.
  */
  int CreateConstraintCollection(int ParentCoID, const MyString& Rank, 
		  const MyString& Constraint);
  /** Same as above, but pass in a classad expression for the rank and 
  	constraint
  */
  int CreateConstraintCollection(int ParentCoID, ExprTree *Rank, 
		  ExprTree *Constraint);

  /** Create a partition collection, as a child of another collection.
      A partiton collection defines a partition based on a set of attributes. 
	  For each distinct set of values (corresponding to these attributes), a new
      child collection will be created, which will contain all the class-ads 
	  from the parent collection that have these values. The partition 
	  collection itself doesn't hold any class-ads, only its children do (the 
	  iteration methods for getting child collections can be used to retrieve 
	  them).
      @param ParentCoID The ID of the parent collection.
      @param Rank The rank expression. Determines how the ads will be ordered 
	  	in the child collections.
      @param AttrList The set of attribute names used to define the partition.
      @return the ID of the new collection, or -1 in case of failure.
  */
  int CreatePartition(int ParentCoID, const MyString& Rank,StringSet& AttrList);
  int CreatePartition(int ParentCoID, ExprTree *Rank, StringSet& AttrList);
  int FindPartition(int ParentCoID, ClassAd *representative );

  /** Deletes a collection and all of its descendants from the collection tree.
      @param CoID The ID of the collection to be deleted.
      @return true on success, false otherwise.
  */
  bool DeleteCollection(int CoID);

  //@}
  //------------------------------------------------------------------------
  /**@name Iteration methods
  */
  //@{

  bool InitializeIterator( int CoID, CollContentIterator& i );
  bool InitializeIterator( int CoID, CollChildIterator& i );

  /** Start iterations on all collections
    @return nothing
  */
  void StartIterateAllCollections();

  /** Get the next collection ID
      @param CoID The ID of the next collection (output parameter).
      @return true on success, false otherwise.
  */
  bool IterateAllCollections(int& CoID);

  /** Start iterations on child collections of a specified collection.
      @param ParentCoID The ID of the parent of the collections to be iterated 
	  	on.
      @return true on success, false otherwise.
  */
  bool StartIterateChildCollections(int ParentCoID);

  /** Get the next child of the specified parent collection.
      @param ParentCoID The ID of the parent of the collections to be iterated 
	  	on.
      @param CoID The ID of the next collection (output parameter).
      @return true on success, false otherwise.
  */
  bool IterateChildCollections(int ParentCoID, int& CoID);

  bool StartIterateClassAds(int CoID);
  bool IterateClassAds( int CoID, ClassAd *& ad );
  bool IterateClassAds( int CoID, char *key );

  /** Start iterations on all class-ads in the repository.
      @return nothing.
  */
  void StartIterateAllClassAds() { table.startIterations(); }

  /** Get the next class-ad in the repository.
      @param Ad A pointer to next the class-ad (output parameter).
      @return true on success, false otherwise.
  */
  bool IterateAllClassAds(ClassAd*& Ad) { return (table.iterate(Ad)==1); }

  //@}
  //------------------------------------------------------------------------
  /**@name Misc methods
  */
  //@{

  /** Find out a collection's type (explicit, constraint, ...).
      @return the type of the specified collection: 0=explicit, 1=constraint.
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

  /// A hash function used by the hash table objects (used internally).
  static int HashFunc(const int& Key, int TableSize) { return (Key % TableSize); }

  //@}
  //------------------------------------------------------------------------

private:
  bool IterateClassAds( int CoID, RankedClassAd &Ranked );
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
  bool ChangeClassAd(const MyString& OID);
  bool RemoveCollection(int CoID, BaseCollection* Coll);
  bool TraverseTree(int CoID, 
		  bool (ClassAdCollection::*Func)(int,BaseCollection*));
  static bool EqualSets(StringSet& S1, StringSet& S2);
  bool CheckClassAd(BaseCollection* Coll, const MyString& OID, ClassAd* Ad);

};

#endif

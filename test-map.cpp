// lang::CwC

#include <gtest/gtest.h>
#include <stdio.h>

#include "map.h"
#include "object.h"
#include "string.h"

void ERR(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

void OUT(const char *msg) {
    printf("%s\n", msg);
}

#define t_true(a)  \
  ASSERT_EQ((a),true);
#define t_false(a) \
  ASSERT_EQ((a),false);
#define t_exit(a)  \
  ASSERT_EXIT(a(), ::testing::ExitedWithCode(0), ".*");

// tests for objects [equal and hash]
void testObject1() {
    OUT("Starting testObject1()...");

    Object *o1 = new Object();
    Object *o2 = new Object();

    t_true(o1->equals(o1));
    t_false(o1->equals(o2));

    t_true(o1->hash() == o1->hash());
    t_false(o1->hash() == o2->hash());

    delete o1;
    delete o2;

    OUT("...passed.");
}

// tests for strings [equal and hash]
void testString1() {
    OUT("Starting testString1()...");

    String *s = new String("hello");
    String *t = new String("world");
    String *u = new String("hello");

    t_true(s->equals(s));
    t_false(s->equals(t));
    t_true(s->equals(u));
    t_true(s->hash() == s->hash());
    t_false(s->hash() == t->hash());
    t_true(s->hash() == u->hash());

    delete s;
    delete t;
    delete u;

    OUT("...passed.");
}

// tests for strings [cmp]
void testString2() {
    OUT("Starting testString2()...");

    String *s = new String("hello");
    String *t = new String("world");
    String *u = new String("hello");
    String *a = new String("apple");

    t_true(s->cmp(s) == 0);
    t_true(s->cmp(u) == 0);
    t_true(s->cmp(t) < 0);
    t_true(s->cmp(a) > 0);
    t_true(t->cmp(u) > 0);

    delete s;
    delete t;
    delete u;
    delete a;

    OUT("...passed.");
}

// tests for strings [concat, size]
void testString3() {
    OUT("Starting testString3()...");

    String *empty = new String("");
    String *s = new String("hello");
    String *t = new String("world");
    String *u = new String("helloworld");
    String *v = new String("worldhello");

    // Testing size(), while we have these allocated.
    t_true(empty->size() == 0);
    t_true(s->size() == 5);
    t_true(t->size() == 5);
    t_true(u->size() == 10);
    t_true(v->size() == 10);

    String *st = s->concat(t);
    t_true(st->equals(u));
    String *ts = t->concat(s);
    t_true(ts->equals(v));

    // Concatenating with the empty string.
    String *s2 = s->concat(empty);
    t_true(s2->equals(s));
    String *s3 = empty->concat(s);
    t_true(s3->equals(s));

    delete empty;
    delete s;
    delete t;
    delete u;
    delete v;
    delete st;
    delete ts;
    delete s2;
    delete s3;

    OUT("...passed.");
}

// tests for Map from Object to Object [get, put, contains_key, len]
void testObjToObjMap1() {
    OUT("Starting testObjToObjMap1()...");

    Map *smap = new Map();
    Object *o1 = new Object();
    Object *o2 = new Object();
    Object *o3 = new Object();
    Object *o4 = new Object();
    t_true(smap->size() == 0);          // initial length of map is zero
    t_true(smap->put(o1, o2) == 0);     // add one key/value pair return 0
    t_true(smap->contains_key(o1));     // contains key added
    t_false(smap->contains_key(o2));    // does not contain a non-added key
    t_true(smap->get(o1)->equals(o2));  // key returns matching value
    t_true(smap->size() == 1);          // length is now 1
    t_true(smap->put(o3, o4) == 0);     // add a second pair
    t_true(smap->contains_key(o3));     // second key is in map
    t_true(smap->get(o3)->equals(o4));  // get second value
    t_true(smap->size() == 2);          // length is now 2
    t_true(smap->put(o1, o3)->equals(
            o2));                           // replace first value, return first value
    t_true(smap->get(o1)->equals(o3));  // new value now exists at first key

    delete smap;
    delete o1;
    delete o2;
    delete o3;
    delete o4;

    OUT("...passed.");
}

// tests for Map from Object to Object [put_all]
void testObjToObjMap2() {
    OUT("Starting testObjToObjMap2()...");

    Map *a = new Map();
    Map *b = new Map();
    Map *c = new Map();
    Object *o1 = new Object();
    Object *o2 = new Object();
    Object *o3 = new Object();
    a->put(o1, o2);              // puts key/val pair into a
    c->put(o1, o2);              // puts same key/val pair into c
    t_true(b->put_all(a) == 0);  // put_all of a into b returns 0
    a->put(o1, o3);              // replaces value in a
    t_true(b->put_all(a)->equals(
            c));  // put_all of updated a into b returns map with same pair as c

    delete a;
    delete b;
    delete c;
    delete o1;
    delete o2;
    delete o3;

    OUT("...passed.");
}

// tests for Map from String to Object [get, put, contains_key, len]
void testStrToObjMap1() {
    OUT("Starting testStrToObjMap1()...");

    Map *smap = new Map();
    String *s = new String("key");
    String *t = new String("another key");
    String *u = new String("yet another one");
    Object *o1 = new Object();
    Object *o2 = new Object();
    t_true(smap->size() == 0);         // initial length of map is zero
    t_true(smap->put(s, o1) == 0);     // add one key/value pair return 0
    t_true(smap->contains_key(s));     // contains key added
    t_false(smap->contains_key(u));    // does not contain a non-added key
    t_true(smap->get(s)->equals(o1));  // key returns matching value
    t_true(smap->size() == 1);         // length is now 1
    t_true(smap->put(t, o2) == 0);     // add a second pair
    t_true(smap->contains_key(t));     // second key is in map
    t_true(smap->get(t)->equals(o2));  // get second value
    t_true(smap->size() == 2);         // length is now 2
    t_true(smap->put(s, o2)->equals(o1));  // replace first value, return first value
    t_true(smap->get(s)->equals(o2));   // new value now exists at first key

    delete smap;
    delete s;
    delete t;
    delete u;
    delete o1;
    delete o2;

    OUT("...passed.");
}

// tests for Map from String to Object [put_all]
void testStrToObjMap2() {
    OUT("Starting testStrToObjMap2()...");

    Map *a = new Map();
    Map *b = new Map();
    Map *c = new Map();
    String *k = new String("a key");
    Object *o1 = new Object();
    Object *o2 = new Object();
    a->put(k, o1);               // puts key/val pair into a
    c->put(k, o1);               // puts same key/val pair into c
    t_true(b->put_all(a) == 0);  // put_all of a into b returns 0
    a->put(k, o2);               // replaces value in a
    t_true(b->put_all(a)->equals(
            c));  // put_all of updated a into b returns map with same pair as c

    delete a;
    delete b;
    delete c;
    delete k;
    delete o1;
    delete o2;

    OUT("...passed.");
}

// tests for StrToStrMap [get, put, contains_key, len]
void testStrToStrMap1() {
    OUT("Starting testStrToStrMap1()...");

    StrToStrMap *smap = new StrToStrMap();
    String *k = new String("key");
    String *v = new String("value");
    String *u = new String("unused");
    String *k2 = new String("second key");

    t_true(smap->size() == 0);         // initial length of map is zero
    t_true(smap->put(k, v) == 0);      // add one key/value pair return 0
    t_true(smap->contains_key(k));     // contains key added
    t_false(smap->contains_key(u));    // does not contain a non-added key
    t_true(smap->get(k)->equals(v));   // key returns matching value
    t_true(smap->size() == 1);         // length is now 1
    t_true(smap->put(k2, u) == 0);     // add a second pair
    t_true(smap->contains_key(k2));    // second key is in map
    t_true(smap->get(k2)->equals(u));  // get second value
    t_true(smap->size() == 2);         // length is now 2
    t_true(
            smap->put(k, u)->equals(v));  // replace first value, return first value
    t_true(smap->get(k)->equals(u));  // new value now exists at first key
    t_false(smap->get(k)->equals(v));

    delete smap;
    delete k;
    delete v;
    delete u;
    delete k2;

    OUT("...passed.");
}

// tests for StrToStrMap put_all
void testStrToStrMap2() {
    OUT("Starting testStrToStrMap2()...");

    StrToStrMap *a = new StrToStrMap();
    StrToStrMap *b = new StrToStrMap();
    StrToStrMap *c = new StrToStrMap();
    String *ak = new String("a key");
    String *av = new String("a value");
    String *n = new String("new value");
    a->put(ak, av);              // puts key/val pair into a
    c->put(ak, av);              // puts same key/val pair into c
    t_true(b->put_all(a) == 0);  // put_all of a into b returns 0
    a->put(ak, n);               // replaces value in a
    t_true(b->put_all(a)->equals(
            c));  // put_all of updated a into b returns map with same pair as c

    delete a;
    delete b;
    delete c;
    delete ak;
    delete av;
    delete n;

    OUT("...passed.");
}

int main() {
    OUT("Beginning tests!");

    testObject1();
    testString1();
    testString2();
    testString3();
    testObjToObjMap1();
    testObjToObjMap2();
    testStrToObjMap1();
    testStrToObjMap2();
    testStrToStrMap1();
    testStrToStrMap2();

    OUT("Passed all tests! :)");

    return 0;
}

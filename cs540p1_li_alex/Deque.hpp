#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Deque_DEFINE(t)                                                 \
    struct Deque_##t##_Iterator;                                        \
    struct Deque_##t {                                                  \
        t *array;                                                       \
        int head;                                                       \
        int tail;                                                       \
        int max;                                                        \
        int dsize;                                                      \
        char type_name[6 + (int)sizeof(#t)];                            \
        int (*size)(Deque_##t *);                                       \
        bool (*empty)(Deque_##t *);                                     \
        void (*push_back)(Deque_##t *, t value);                        \
        void (*push_front)(Deque_##t *, t value);                       \
        t &(*front)(Deque_##t *);                                       \
        t &(*back)(Deque_##t *);                                        \
        void (*pop_front)(Deque_##t *);                                 \
        void (*pop_back)(Deque_##t *);                                  \
        t &(*at)(Deque_##t *, int i);                                   \
        void (*clear)(Deque_##t *);                                     \
        void (*dtor)(Deque_##t *);                                      \
        bool (*equal)(Deque_##t &,Deque_##t &);                         \
        Deque_##t##_Iterator (*begin)(Deque_##t *);                     \
        Deque_##t##_Iterator (*end)(Deque_##t *);                       \
        bool (*equality)(const t &, const t &);                         \
    };                                                                  \
    struct Deque_##t##_Iterator{                                        \
        int index;                                                      \
        Deque_##t *itr;                                                 \
        t &(*deref)(Deque_##t##_Iterator *);                            \
        void (*inc)(Deque_##t##_Iterator *);                            \
        void (*dec)(Deque_##t##_Iterator *);                            \
        bool (*equal)(const Deque_##t##_Iterator &, const Deque_##t##_Iterator &);  \
    };                                                                  \
    int Deque_##t##_size(Deque_##t *d){                                 \
        return d->dsize;                                                \
    }                                                                   \
    bool Deque_##t##_empty(Deque_##t *d){                               \
        return d->dsize == 0;                                           \
    }                                                                   \
    void Deque_##t##_push_back(Deque_##t *d, t value){                  \
        if(d->dsize == d->max){                                         \
            int index = 0;                                              \
            t *temp = d->array;                                         \
            d->array = (t*)malloc(d->max * 2 * sizeof(t));              \
            for(int i = 0; i < d->max; i++){                            \
                index = d->head + i;                                    \
                if(index >= d->max){                                    \
                    index -= d->max;                                    \
                }                                                       \
                d->array[i] = temp[index];                              \
            }                                                           \
            d->array[d->max] = value;                                   \
            d->head = 0;                                                \
            d->tail = d->max;                                           \
            d->max *= 2;                                                \
            d->dsize++;                                                 \
            free(temp);                                                 \
        }                                                               \
        else{                                                           \
            if(d->dsize == 0){                                          \
                d->dsize++;                                             \
                d->array[0] = value;                                    \
                d->head = 0;                                            \
                d->tail = 0;                                            \
            }                                                           \
            else{                                                       \
                d->dsize++;                                             \
                d->tail++;                                              \
                if(d->tail >= d->max){                                  \
                    d->tail -= d->max;                                  \
                }                                                       \
                d->array[d->tail] = value;                              \
            }                                                           \
        }                                                               \
    }                                                                   \
    void Deque_##t##_push_front(Deque_##t *d, t value){                 \
        if(d->dsize == d->max){                                         \
            int index = 0;                                              \
            t *temp = d->array;                                         \
            d->array = (t*)malloc(d->max * 2 * sizeof(t));              \
            d->array[0] = value;                                        \
            for(int i = 0; i < d->max; i++){                            \
                index = d->head + i;                                    \
                if(index >= d->max){                                    \
                    index -= d->max;                                    \
                }                                                       \
                d->array[i + 1] = temp[index];                          \
            }                                                           \
            d->head = 0;                                                \
            d->tail = d->max;                                           \
            d->max *= 2;                                                \
            d->dsize++;                                                 \
            free(temp);                                                 \
        }                                                               \
        else{                                                           \
            if(d->dsize == 0){                                          \
                d->dsize++;                                             \
                d->array[0] = value;                                    \
                d->head = 0;                                            \
                d->tail = 0;                                            \
            }                                                           \
            else{                                                       \
                d->dsize++;                                             \
                d->head--;                                              \
                if(d->head < 0){                                        \
                    d->head += d->max;                                  \
                }                                                       \
                d->array[d->head] = value;                              \
            }                                                           \
        }                                                               \
    }                                                                   \
    t &Deque_##t##_front(Deque_##t *d){                                 \
        return d->array[d->head];                                       \
    }                                                                   \
    t &Deque_##t##_back(Deque_##t *d){                                  \
        return d->array[d->tail];                                       \
    }                                                                   \
    void Deque_##t##_pop_front(Deque_##t *d){                           \
        if(d->dsize > 0){                                               \
            if(d->dsize == 1){                                          \
                d->dsize--;                                             \
                d->head = 0;                                            \
                d->tail = 0;                                            \
            }                                                           \
            else{                                                       \
                d->dsize--;                                             \
                d->head++;                                              \
                if(d->head >= d->max){                                  \
                    d->head -= d->max;                                  \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
    void Deque_##t##_pop_back(Deque_##t *d){                            \
        if(d->dsize > 0){                                               \
            if(d->dsize == 1){                                          \
                d->dsize--;                                             \
                d->head = 0;                                            \
                d->tail = 0;                                            \
            }                                                           \
            else{                                                       \
                d->dsize--;                                             \
                d->tail--;                                              \
                if(d->tail < 0){                                        \
                    d->tail += d->max;                                  \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
    t &Deque_##t##_at(Deque_##t *d, int i){                             \
        if((i >= 0) && (i < d->dsize)){                                 \
            int temp = d->head;                                         \
            temp += i;                                                  \
            if(temp >= d->max){                                         \
                temp -= d->max;                                         \
            }                                                           \
            return d->array[temp];                                      \
        }                                                               \
        else{                                                           \
            exit(1);                                                    \
        }                                                               \
    }                                                                   \
    void Deque_##t##_clear(Deque_##t *d){                               \
        d->size = 0;                                                    \
        d->head = 0;                                                    \
        d->tail = 0;                                                    \
        memset(d->array, 0, d->max * sizeof(t));                        \
    }                                                                   \
    void Deque_##t##_dtor(Deque_##t *d){                                \
        free(d->array);                                                 \
        d = nullptr;                                                    \
    }                                                                   \
    bool Deque_##t##_equal(Deque_##t &d1, Deque_##t &d2){   \
        if(d1.dsize == d2.dsize){                                     \
            for(int i = 0; i < d1.dsize; i++){                         \
                if(d1.equality(d1.at(&d1, i), d2.at(&d2,i)) || d2.equality(d2.at(&d2, i), d1.at(&d1, i))){                                                \
                    return false;                                       \
                }                                                       \
            }                                                           \
            return true;                                                \
        }                                                               \
        return false;                                                   \
    }                                                                   \
    t &Deque_##t##_Iterator_deref(Deque_##t##_Iterator *it){            \
        return it->itr->array[it->index];                               \
    }                                                                   \
    void Deque_##t##_Iterator_inc(Deque_##t##_Iterator *it){            \
        if(it->index == it->itr->tail){                                 \
            it->index = -1;                                             \
        }                                                               \
        else{                                                           \
            it->index++;                                                \
            if(it->index >= it->itr->max){                              \
                it->index -= it->itr->max;                              \
            }                                                           \
        }                                                               \
    }                                                                   \
    void Deque_##t##_Iterator_dec(Deque_##t##_Iterator *it){            \
        if(it->index == -1){                                            \
            it->index = it->itr->tail;                                  \
        }                                                               \
        else{                                                           \
            it->index--;                                                \
            if(it->index < 0){                                          \
                it->index += it->itr->max;                              \
            }                                                           \
        }                                                               \
    }                                                                   \
    bool Deque_##t##_Iterator_equal(const Deque_##t##_Iterator &it1, const Deque_##t##_Iterator &it2){\
        if(it1.itr == it2.itr){                                         \
            if(it1.index == it2.index){                                 \
                return true;                                            \
            }                                                           \
        }                                                               \
        return false;                                                   \
    }                                                                   \
    Deque_##t##_Iterator Deque_##t##_begin(Deque_##t *d){               \
        Deque_##t##_Iterator it;                                        \
        it.index = d->head;                                             \
        it.itr = d;                                                     \
        it.deref = &Deque_##t##_Iterator_deref;                         \
        it.inc = &Deque_##t##_Iterator_inc;                             \
        it.dec = &Deque_##t##_Iterator_dec;                             \
        it.equal = &Deque_##t##_Iterator_equal;                         \
        return it;                                                      \
    }                                                                   \
    Deque_##t##_Iterator Deque_##t##_end(Deque_##t *d){                 \
        Deque_##t##_Iterator it;                                        \
        it.index = -1;                                                  \
        it.itr = d;                                                     \
        it.deref = &Deque_##t##_Iterator_deref;                         \
        it.inc = &Deque_##t##_Iterator_inc;                             \
        it.dec = &Deque_##t##_Iterator_dec;                             \
        it.equal = &Deque_##t##_Iterator_equal;                         \
        return it;                                                      \
    }                                                                   \
    void Deque_##t##_ctor(Deque_##t *d, bool (*less)(const t &, const t &)) {\
        d->array = (t*)malloc(10 * sizeof(t));                          \
        d->head = 0;                                                    \
        d->tail = 0;                                                    \
        d->dsize = 0;                                                   \
        d->max = 10;                                                    \
        strcpy(d->type_name, "Deque_" #t);                              \
        d->size = &Deque_##t##_size;                                    \
        d->empty = &Deque_##t##_empty;                                  \
        d->push_back = &Deque_##t##_push_back;                          \
        d->push_front = &Deque_##t##_push_front;                        \
        d->front = &Deque_##t##_front;                                  \
        d->back = &Deque_##t##_back;                                    \
        d->pop_front = &Deque_##t##_pop_front;                          \
        d->pop_back = &Deque_##t##_pop_back;                            \
        d->at = &Deque_##t##_at;                                        \
        d->clear = &Deque_##t##_clear;                                  \
        d->dtor = &Deque_##t##_dtor;                                    \
        d->equal = &Deque_##t##_equal;                                  \
        d->begin = &Deque_##t##_begin;                                  \
        d->end = &Deque_##t##_end;                                      \
        d->equality = less;                                             \
    }                                                                   \

#endif /* DEQUE_HPP */
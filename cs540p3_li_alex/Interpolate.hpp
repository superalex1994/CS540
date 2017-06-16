#ifndef INTERPOLATE_HPP
#define INTERPOLATE_HPP

#include <tuple>
#include <string>
#include <iostream>
#include <exception>
#include <iomanip>

using namespace std;

namespace cs540{
    
    class WrongNumberOfArgs: public exception{};
    
    // Typedef
    typedef std::ios_base &(*fp1)(std::ios_base &);
    typedef std::ostream &(*fp2)(std::ostream &);
    
    // Forward declarations
    void unpack2(ostream &os, const string s, size_t start);
    template<typename T, typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, const T &v, const Ts &... rest);
    template<typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, fp1 f, const Ts &... rest);
    template<typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, fp2 f, const Ts &... rest);
    template<typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, std::result_of<decltype(std::resetiosflags)&(typename std::ios_base::fmtflags)>::type f, const Ts &... rest);
    template<typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, std::result_of<decltype(std::setiosflags)&(typename std::ios_base::fmtflags)>::type f, const Ts &... rest);
    template<typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, std::result_of<decltype(std::setbase)&(int)>::type f, const Ts &... rest);
    
    template<typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, std::result_of<decltype(std::setprecision)&(int)>::type f, const Ts &... rest);
    template<typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, std::result_of<decltype(std::setw)&(int)>::type f, const Ts &... rest);
    
    // Tuple unpacking
    // No more arguments
    void unpack2(ostream &os, const string s, size_t start){
        int spos = start;
        size_t epos = s.find("%", spos);
        if(epos != string::npos){
            while(epos != string::npos){
                if(s[epos-1] != '\\'){ // Too few arguments
                    throw cs540::WrongNumberOfArgs();
                }
                // Escape sequence
                os << s.substr(spos, (epos-spos) - 1);
                os << "%";
                spos = epos + 1;
                epos = s.find("%", spos);
            }
            
        }
        else{ // Output rest of the string
            os << s.substr(start);
        }
    }

    // Detect and deal with ios manipulators
    template<typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, fp1 f, const Ts &... rest){
        os << f;
        unpack2(os, s, start, rest...);
    }
    
    // Detect and deal with ostream manipulators
    template<typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, fp2 f, const Ts &... rest){
        os << f;
        unpack2(os, s, start, rest...);
    }
    
    // Setiosflags
    template<typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, std::result_of<decltype(std::setiosflags)&(typename std::ios_base::fmtflags)>::type f, const Ts &... rest){
        os << f;
        unpack2(os, s, start, rest...);
    }
    
    // Resetiosflags
    template<typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, std::result_of<decltype(std::resetiosflags)&(typename std::ios_base::fmtflags)>::type f, const Ts &... rest){
        os << f;
        unpack2(os, s, start, rest...);
    }
    
    // Setbase
    template<typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, std::result_of<decltype(std::setbase)&(int)>::type f, const Ts &... rest){
        os << f;
        unpack2(os, s, start, rest...);
    }
    
    // Setfill
    //template<class CharT, typename... Ts>
    //void unpack2(ostream &os, const string s, size_t start, std::result_of<decltype(std::setfill)&(class CharT)>::type f, const Ts &... rest){
        //os << f;
        //unpack2(os, s, start, rest...);
    //}
    
    // Setprecision
    template<typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, std::result_of<decltype(std::setprecision)&(int)>::type f, const Ts &... rest){
        os << f;
        unpack2(os, s, start, rest...);
    }
    
    // Setw
    template<typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, std::result_of<decltype(std::setw)&(int)>::type f, const Ts &... rest){
        os << f;
        unpack2(os, s, start, rest...);
    }
    
    template<typename T, typename... Ts>
    void unpack2(ostream &os, const string s, size_t start, const T &v, const Ts &... rest){
        string delim = "%";
        size_t end = s.find(delim, start);
        if(end != string::npos){
            if(s[end-1] == '\\'){ // Escape sequence
                os << s.substr(start, (end-start) - 1);
                os << "%";
                unpack2(os, s, end+1, v, rest...);
            }
            else{ //Consume % and replace with argument
                os << s.substr(start, end-start);
                os << v;
                unpack2(os, s, end+1, rest...);
            }
        }
        else{ // Too many arguments
            throw cs540::WrongNumberOfArgs();
        }
    }
    
    template<typename T, std::size_t... ls>
    void unpack(ostream &os, const string s, const T &t, const std::index_sequence<ls...> &){
        unpack2(os, s, 0, std::get<ls>(t)...);
    }
    
    template<typename... Ts>
    class Helper{
        friend ostream &operator<<(ostream &os, const Helper<Ts...> &h){
            unpack(os, h.s, h.t, std::make_index_sequence<std::tuple_size<decltype(h.t)>::value>());
            return os;
        }
    public:
        const string s;
        tuple<Ts...> t;
        Helper(const string s1, const Ts &... args): s(s1), t(args...) {}
    };
    
    template<typename... Ts>
    Helper<const Ts &...> Interpolate(const string s, const Ts &... args){
        Helper<const Ts &...> h(s, args...);
        return h;
    }
}
#endif
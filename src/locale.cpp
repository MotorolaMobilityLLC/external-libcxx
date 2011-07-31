//===------------------------- locale.cpp ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "string"
#include "locale"
#include "codecvt"
#include "vector"
#include "algorithm"
#include "algorithm"
#include "typeinfo"
#include "type_traits"
#include "clocale"
#include "cstring"
#include "cwctype"
#include "__sso_allocator"
#include <langinfo.h>
#include <stdlib.h>

#ifdef _LIBCPP_STABLE_APPLE_ABI
namespace {
  decltype(MB_CUR_MAX_L(_VSTD::declval<locale_t>()))
  inline _LIBCPP_INLINE_VISIBILITY
  mb_cur_max_l(locale_t loc)
  {
    return MB_CUR_MAX_L(loc);
  }
}
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#ifndef _LIBCPP_STABLE_APPLE_ABI
locale_t __cloc() {
  // In theory this could create a race condition. In practice
  // the race condition is non-fatal since it will just create
  // a little resource leak. Better approach would be appreciated.
#ifdef __APPLE__
  return 0;
#else
  static locale_t result = newlocale(LC_ALL_MASK, "C", 0);
  return result;
#endif
}
#endif // _LIBCPP_STABLE_APPLE_ABI

namespace {

struct release
{
    void operator()(locale::facet* p) {p->__release_shared();}
};

template <class T, class A0>
inline
T&
make(A0 a0)
{
    static typename aligned_storage<sizeof(T)>::type buf;
    ::new (&buf) T(a0);
    return *(T*)&buf;
}

template <class T, class A0, class A1>
inline
T&
make(A0 a0, A1 a1)
{
    static typename aligned_storage<sizeof(T)>::type buf;
    ::new (&buf) T(a0, a1);
    return *(T*)&buf;
}

template <class T, class A0, class A1, class A2>
inline
T&
make(A0 a0, A1 a1, A2 a2)
{
    static typename aligned_storage<sizeof(T)>::type buf;
    ::new (&buf) T(a0, a1, a2);
    return *(T*)&buf;
}

}

class _LIBCPP_HIDDEN locale::__imp
    : public facet
{
    enum {N = 28};
    string         name_;
    vector<facet*, __sso_allocator<facet*, N> > facets_;
public:
    explicit __imp(size_t refs = 0);
    explicit __imp(const string& name, size_t refs = 0);
    __imp(const __imp&);
    __imp(const __imp&, const string&, locale::category c);
    __imp(const __imp& other, const __imp& one, locale::category c);
    __imp(const __imp&, facet* f, long id);
    ~__imp();

    const string& name() const {return name_;}
    bool has_facet(long id) const {return id < facets_.size() && facets_[id];}
    const locale::facet* use_facet(long id) const;

    static const locale& make_classic();
    static       locale& make_global();
private:
    void install(facet* f, long id);
    template <class F> void install(F* f) {install(f, f->id.__get());}
    template <class F> void install_from(const __imp& other);
};

locale::__imp::__imp(size_t refs)
    : facet(refs),
      name_("C"),
      facets_(N)
{
    facets_.clear();
    install(&make<_VSTD::collate<char> >(1));
    install(&make<_VSTD::collate<wchar_t> >(1));
    install(&make<_VSTD::ctype<char> >((ctype_base::mask*)0, false, 1));
    install(&make<_VSTD::ctype<wchar_t> >(1));
    install(&make<codecvt<char, char, mbstate_t> >(1));
    install(&make<codecvt<wchar_t, char, mbstate_t> >(1));
    install(&make<codecvt<char16_t, char, mbstate_t> >(1));
    install(&make<codecvt<char32_t, char, mbstate_t> >(1));
    install(&make<numpunct<char> >(1));
    install(&make<numpunct<wchar_t> >(1));
    install(&make<num_get<char> >(1));
    install(&make<num_get<wchar_t> >(1));
    install(&make<num_put<char> >(1));
    install(&make<num_put<wchar_t> >(1));
    install(&make<moneypunct<char, false> >(1));
    install(&make<moneypunct<char, true> >(1));
    install(&make<moneypunct<wchar_t, false> >(1));
    install(&make<moneypunct<wchar_t, true> >(1));
    install(&make<money_get<char> >(1));
    install(&make<money_get<wchar_t> >(1));
    install(&make<money_put<char> >(1));
    install(&make<money_put<wchar_t> >(1));
    install(&make<time_get<char> >(1));
    install(&make<time_get<wchar_t> >(1));
    install(&make<time_put<char> >(1));
    install(&make<time_put<wchar_t> >(1));
    install(&make<_VSTD::messages<char> >(1));
    install(&make<_VSTD::messages<wchar_t> >(1));
}

locale::__imp::__imp(const string& name, size_t refs)
    : facet(refs),
      name_(name),
      facets_(N)
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        facets_ = locale::classic().__locale_->facets_;
        for (unsigned i = 0; i < facets_.size(); ++i)
            if (facets_[i])
                facets_[i]->__add_shared();
        install(new collate_byname<char>(name_));
        install(new collate_byname<wchar_t>(name_));
        install(new ctype_byname<char>(name_));
        install(new ctype_byname<wchar_t>(name_));
        install(new codecvt_byname<char, char, mbstate_t>(name_));
        install(new codecvt_byname<wchar_t, char, mbstate_t>(name_));
        install(new codecvt_byname<char16_t, char, mbstate_t>(name_));
        install(new codecvt_byname<char32_t, char, mbstate_t>(name_));
        install(new numpunct_byname<char>(name_));
        install(new numpunct_byname<wchar_t>(name_));
        install(new moneypunct_byname<char, false>(name_));
        install(new moneypunct_byname<char, true>(name_));
        install(new moneypunct_byname<wchar_t, false>(name_));
        install(new moneypunct_byname<wchar_t, true>(name_));
        install(new time_get_byname<char>(name_));
        install(new time_get_byname<wchar_t>(name_));
        install(new time_put_byname<char>(name_));
        install(new time_put_byname<wchar_t>(name_));
        install(new messages_byname<char>(name_));
        install(new messages_byname<wchar_t>(name_));
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        for (unsigned i = 0; i < facets_.size(); ++i)
            if (facets_[i])
                facets_[i]->__release_shared();
        throw;
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
}

locale::__imp::__imp(const __imp& other)
    : name_(other.name_),
      facets_(max<size_t>(N, other.facets_.size()))
{
    facets_ = other.facets_;
    for (unsigned i = 0; i < facets_.size(); ++i)
        if (facets_[i])
            facets_[i]->__add_shared();
}

locale::__imp::__imp(const __imp& other, const string& name, locale::category c)
    : name_("*"),
      facets_(N)
{
    facets_ = other.facets_;
    for (unsigned i = 0; i < facets_.size(); ++i)
        if (facets_[i])
            facets_[i]->__add_shared();
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        if (c & locale::collate)
        {
            install(new collate_byname<char>(name));
            install(new collate_byname<wchar_t>(name));
        }
        if (c & locale::ctype)
        {
            install(new ctype_byname<char>(name));
            install(new ctype_byname<wchar_t>(name));
            install(new codecvt_byname<char, char, mbstate_t>(name));
            install(new codecvt_byname<wchar_t, char, mbstate_t>(name));
            install(new codecvt_byname<char16_t, char, mbstate_t>(name));
            install(new codecvt_byname<char32_t, char, mbstate_t>(name));
        }
        if (c & locale::monetary)
        {
            install(new moneypunct_byname<char, false>(name));
            install(new moneypunct_byname<char, true>(name));
            install(new moneypunct_byname<wchar_t, false>(name));
            install(new moneypunct_byname<wchar_t, true>(name));
        }
        if (c & locale::numeric)
        {
            install(new numpunct_byname<char>(name));
            install(new numpunct_byname<wchar_t>(name));
        }
        if (c & locale::time)
        {
            install(new time_get_byname<char>(name));
            install(new time_get_byname<wchar_t>(name));
            install(new time_put_byname<char>(name));
            install(new time_put_byname<wchar_t>(name));
        }
        if (c & locale::messages)
        {
            install(new messages_byname<char>(name));
            install(new messages_byname<wchar_t>(name));
        }
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        for (unsigned i = 0; i < facets_.size(); ++i)
            if (facets_[i])
                facets_[i]->__release_shared();
        throw;
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
}

template<class F>
inline
void
locale::__imp::install_from(const locale::__imp& one)
{
    long id = F::id.__get();
    install(const_cast<F*>(static_cast<const F*>(one.use_facet(id))), id);
}

locale::__imp::__imp(const __imp& other, const __imp& one, locale::category c)
    : name_("*"),
      facets_(N)
{
    facets_ = other.facets_;
    for (unsigned i = 0; i < facets_.size(); ++i)
        if (facets_[i])
            facets_[i]->__add_shared();
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        if (c & locale::collate)
        {
            install_from<_VSTD::collate<char> >(one);
            install_from<_VSTD::collate<wchar_t> >(one);
        }
        if (c & locale::ctype)
        {
            install_from<_VSTD::ctype<char> >(one);
            install_from<_VSTD::ctype<wchar_t> >(one);
            install_from<_VSTD::codecvt<char, char, mbstate_t> >(one);
            install_from<_VSTD::codecvt<char16_t, char, mbstate_t> >(one);
            install_from<_VSTD::codecvt<char32_t, char, mbstate_t> >(one);
            install_from<_VSTD::codecvt<wchar_t, char, mbstate_t> >(one);
        }
        if (c & locale::monetary)
        {
            install_from<moneypunct<char, false> >(one);
            install_from<moneypunct<char, true> >(one);
            install_from<moneypunct<wchar_t, false> >(one);
            install_from<moneypunct<wchar_t, true> >(one);
            install_from<money_get<char> >(one);
            install_from<money_get<wchar_t> >(one);
            install_from<money_put<char> >(one);
            install_from<money_put<wchar_t> >(one);
        }
        if (c & locale::numeric)
        {
            install_from<numpunct<char> >(one);
            install_from<numpunct<wchar_t> >(one);
            install_from<num_get<char> >(one);
            install_from<num_get<wchar_t> >(one);
            install_from<num_put<char> >(one);
            install_from<num_put<wchar_t> >(one);
        }
        if (c & locale::time)
        {
            install_from<time_get<char> >(one);
            install_from<time_get<wchar_t> >(one);
            install_from<time_put<char> >(one);
            install_from<time_put<wchar_t> >(one);
        }
        if (c & locale::messages)
        {
            install_from<_VSTD::messages<char> >(one);
            install_from<_VSTD::messages<wchar_t> >(one);
        }
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        for (unsigned i = 0; i < facets_.size(); ++i)
            if (facets_[i])
                facets_[i]->__release_shared();
        throw;
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
}

locale::__imp::__imp(const __imp& other, facet* f, long id)
    : name_("*"),
      facets_(max<size_t>(N, other.facets_.size()+1))
{
    f->__add_shared();
    unique_ptr<facet, release> hold(f);
    facets_ = other.facets_;
    for (unsigned i = 0; i < other.facets_.size(); ++i)
        if (facets_[i])
            facets_[i]->__add_shared();
    install(hold.get(), id);
}

locale::__imp::~__imp()
{
    for (unsigned i = 0; i < facets_.size(); ++i)
        if (facets_[i])
            facets_[i]->__release_shared();
}

void
locale::__imp::install(facet* f, long id)
{
    f->__add_shared();
    unique_ptr<facet, release> hold(f);
    if (id >= facets_.size())
        facets_.resize(id+1);
    if (facets_[id])
        facets_[id]->__release_shared();
    facets_[id] = hold.release();
}

const locale::facet*
locale::__imp::use_facet(long id) const
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (!has_facet(id))
        throw bad_cast();
#endif  // _LIBCPP_NO_EXCEPTIONS
    return facets_[id];
}

// locale

const locale&
locale::__imp::make_classic()
{
    // only one thread can get in here and it only gets in once
    static aligned_storage<sizeof(locale)>::type buf;
    locale* c = (locale*)&buf;
    c->__locale_ = &make<__imp>(1);
    return *c;
}

const locale&
locale::classic()
{
    static const locale& c = __imp::make_classic();
    return c;
}

locale&
locale::__imp::make_global()
{
    // only one thread can get in here and it only gets in once
    static aligned_storage<sizeof(locale)>::type buf;
    locale* g = (locale*)&buf;
    ::new (&buf) locale(locale::classic());
    return *(locale*)&buf;
}

locale&
locale::__global()
{
    static locale& g = __imp::make_global();
    return g;
}

locale::locale()  _NOEXCEPT
    : __locale_(__global().__locale_)
{
    __locale_->__add_shared();
}

locale::locale(const locale& l)  _NOEXCEPT
    : __locale_(l.__locale_)
{
    __locale_->__add_shared();
}

locale::~locale()
{
    __locale_->__release_shared();
}

const locale&
locale::operator=(const locale& other)  _NOEXCEPT
{
    other.__locale_->__add_shared();
    __locale_->__release_shared();
    __locale_ = other.__locale_;
    return *this;
}

locale::locale(const char* name)
#ifndef _LIBCPP_NO_EXCEPTIONS
    : __locale_(name ? new __imp(name)
                     : throw runtime_error("locale constructed with null"))
#else  // _LIBCPP_NO_EXCEPTIONS
    : __locale_(new __imp(name))
#endif
{
    __locale_->__add_shared();
}

locale::locale(const string& name)
    : __locale_(new __imp(name))
{
    __locale_->__add_shared();
}

locale::locale(const locale& other, const char* name, category c)
#ifndef _LIBCPP_NO_EXCEPTIONS
    : __locale_(name ? new __imp(*other.__locale_, name, c)
                     : throw runtime_error("locale constructed with null"))
#else  // _LIBCPP_NO_EXCEPTIONS
    : __locale_(new __imp(*other.__locale_, name, c))
#endif
{
    __locale_->__add_shared();
}

locale::locale(const locale& other, const string& name, category c)
    : __locale_(new __imp(*other.__locale_, name, c))
{
    __locale_->__add_shared();
}

locale::locale(const locale& other, const locale& one, category c)
    : __locale_(new __imp(*other.__locale_, *one.__locale_, c))
{
    __locale_->__add_shared();
}

string
locale::name() const
{
    return __locale_->name();
}

void
locale::__install_ctor(const locale& other, facet* f, long id)
{
    if (f)
        __locale_ = new __imp(*other.__locale_, f, id);
    else
        __locale_ = other.__locale_;
    __locale_->__add_shared();
}

locale
locale::global(const locale& loc)
{
    locale& g = __global();
    locale r = g;
    g = loc;
    if (g.name() != "*")
        setlocale(LC_ALL, g.name().c_str());
    return r;
}

bool
locale::has_facet(id& x) const
{
    return __locale_->has_facet(x.__get());
}

const locale::facet*
locale::use_facet(id& x) const
{
    return __locale_->use_facet(x.__get());
}

bool
locale::operator==(const locale& y) const
{
    return (__locale_ == y.__locale_)
        || (__locale_->name() != "*" && __locale_->name() == y.__locale_->name());
}

// locale::facet

locale::facet::~facet()
{
}

void
locale::facet::__on_zero_shared() _NOEXCEPT
{
    delete this;
}

// locale::id

int32_t locale::id::__next_id = 0;

namespace
{

class __fake_bind
{
    locale::id* id_;
    void (locale::id::* pmf_)();
public:
    __fake_bind(void (locale::id::* pmf)(), locale::id* id)
        : id_(id), pmf_(pmf) {}

    void operator()() const
    {
        (id_->*pmf_)();
    }
};

}

long
locale::id::__get()
{
    call_once(__flag_, __fake_bind(&locale::id::__init, this));
    return __id_ - 1;
}

void
locale::id::__init()
{
    __id_ = __sync_add_and_fetch(&__next_id, 1);
}

// template <> class collate_byname<char>

collate_byname<char>::collate_byname(const char* n, size_t refs)
    : collate<char>(refs),
      __l(newlocale(LC_ALL_MASK, n, 0))
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__l == 0)
        throw runtime_error("collate_byname<char>::collate_byname"
                            " failed to construct for " + string(n));
#endif  // _LIBCPP_NO_EXCEPTIONS
}

collate_byname<char>::collate_byname(const string& name, size_t refs)
    : collate<char>(refs),
      __l(newlocale(LC_ALL_MASK, name.c_str(), 0))
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__l == 0)
        throw runtime_error("collate_byname<char>::collate_byname"
                            " failed to construct for " + name);
#endif  // _LIBCPP_NO_EXCEPTIONS
}

collate_byname<char>::~collate_byname()
{
    freelocale(__l);
}

int
collate_byname<char>::do_compare(const char_type* __lo1, const char_type* __hi1,
                                 const char_type* __lo2, const char_type* __hi2) const
{
    string_type lhs(__lo1, __hi1);
    string_type rhs(__lo2, __hi2);
    int r = strcoll_l(lhs.c_str(), rhs.c_str(), __l);
    if (r < 0)
        return -1;
    if (r > 0)
        return 1;
    return r;
}

collate_byname<char>::string_type
collate_byname<char>::do_transform(const char_type* lo, const char_type* hi) const
{
    const string_type in(lo, hi);
    string_type out(strxfrm_l(0, in.c_str(), 0, __l), char());
    strxfrm_l(const_cast<char*>(out.c_str()), in.c_str(), out.size()+1, __l);
    return out;
}

// template <> class collate_byname<wchar_t>

collate_byname<wchar_t>::collate_byname(const char* n, size_t refs)
    : collate<wchar_t>(refs),
      __l(newlocale(LC_ALL_MASK, n, 0))
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__l == 0)
        throw runtime_error("collate_byname<wchar_t>::collate_byname(size_t refs)"
                            " failed to construct for " + string(n));
#endif  // _LIBCPP_NO_EXCEPTIONS
}

collate_byname<wchar_t>::collate_byname(const string& name, size_t refs)
    : collate<wchar_t>(refs),
      __l(newlocale(LC_ALL_MASK, name.c_str(), 0))
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__l == 0)
        throw runtime_error("collate_byname<wchar_t>::collate_byname(size_t refs)"
                            " failed to construct for " + name);
#endif  // _LIBCPP_NO_EXCEPTIONS
}

collate_byname<wchar_t>::~collate_byname()
{
    freelocale(__l);
}

int
collate_byname<wchar_t>::do_compare(const char_type* __lo1, const char_type* __hi1,
                                 const char_type* __lo2, const char_type* __hi2) const
{
    string_type lhs(__lo1, __hi1);
    string_type rhs(__lo2, __hi2);
    int r = wcscoll_l(lhs.c_str(), rhs.c_str(), __l);
    if (r < 0)
        return -1;
    if (r > 0)
        return 1;
    return r;
}

collate_byname<wchar_t>::string_type
collate_byname<wchar_t>::do_transform(const char_type* lo, const char_type* hi) const
{
    const string_type in(lo, hi);
    string_type out(wcsxfrm_l(0, in.c_str(), 0, __l), wchar_t());
    wcsxfrm_l(const_cast<wchar_t*>(out.c_str()), in.c_str(), out.size()+1, __l);
    return out;
}

// template <> class ctype<wchar_t>;

locale::id ctype<wchar_t>::id;

ctype<wchar_t>::~ctype()
{
}

bool
ctype<wchar_t>::do_is(mask m, char_type c) const
{
    return isascii(c) ? ctype<char>::classic_table()[c] & m : false;
}

const wchar_t*
ctype<wchar_t>::do_is(const char_type* low, const char_type* high, mask* vec) const
{
    for (; low != high; ++low, ++vec)
        *vec = static_cast<mask>(isascii(*low) ?
                                   ctype<char>::classic_table()[*low] : 0);
    return low;
}

const wchar_t*
ctype<wchar_t>::do_scan_is(mask m, const char_type* low, const char_type* high) const
{
    for (; low != high; ++low)
        if (isascii(*low) && (ctype<char>::classic_table()[*low] & m))
            break;
    return low;
}

const wchar_t*
ctype<wchar_t>::do_scan_not(mask m, const char_type* low, const char_type* high) const
{
    for (; low != high; ++low)
        if (!(isascii(*low) && (ctype<char>::classic_table()[*low] & m)))
            break;
    return low;
}

wchar_t
ctype<wchar_t>::do_toupper(char_type c) const
{
#ifndef _LIBCPP_STABLE_APPLE_ABI
    return isascii(c) ? ctype<char>::__classic_upper_table()[c] : c;
#else
    return isascii(c) ? _DefaultRuneLocale.__mapupper[c] : c;
#endif
}

const wchar_t*
ctype<wchar_t>::do_toupper(char_type* low, const char_type* high) const
{
    for (; low != high; ++low)
#ifndef _LIBCPP_STABLE_APPLE_ABI
        *low = isascii(*low) ? ctype<char>::__classic_upper_table()[*low]
                             : *low;
#else
        *low = isascii(*low) ? _DefaultRuneLocale.__mapupper[*low] : *low;
#endif
    return low;
}

wchar_t
ctype<wchar_t>::do_tolower(char_type c) const
{
#ifndef _LIBCPP_STABLE_APPLE_ABI
    return isascii(c) ? ctype<char>::__classic_lower_table()[c] : c;
#else
    return isascii(c) ? _DefaultRuneLocale.__maplower[c] : c;
#endif
}

const wchar_t*
ctype<wchar_t>::do_tolower(char_type* low, const char_type* high) const
{
    for (; low != high; ++low)
#ifndef _LIBCPP_STABLE_APPLE_ABI
        *low = isascii(*low) ? ctype<char>::__classic_lower_table()[*low]
                             : *low;
#else
        *low = isascii(*low) ? _DefaultRuneLocale.__maplower[*low] : *low;
#endif
    return low;
}

wchar_t
ctype<wchar_t>::do_widen(char c) const
{
    return c;
}

const char*
ctype<wchar_t>::do_widen(const char* low, const char* high, char_type* dest) const
{
    for (; low != high; ++low, ++dest)
        *dest = *low;
    return low;
}

char
ctype<wchar_t>::do_narrow(char_type c, char dfault) const
{
    if (isascii(c))
        return static_cast<char>(c);
    return dfault;
}

const wchar_t*
ctype<wchar_t>::do_narrow(const char_type* low, const char_type* high, char dfault, char* dest) const
{
    for (; low != high; ++low, ++dest)
        if (isascii(*low))
            *dest = *low;
        else
            *dest = dfault;
    return low;
}

// template <> class ctype<char>;

locale::id ctype<char>::id;

ctype<char>::ctype(const mask* tab, bool del, size_t refs)
    : locale::facet(refs),
      __tab_(tab),
      __del_(del)
{
  if (__tab_ == 0)
      __tab_ = classic_table();
}

ctype<char>::~ctype()
{
    if (__tab_ && __del_)
        delete [] __tab_;
}

char
ctype<char>::do_toupper(char_type c) const
{
#ifndef _LIBCPP_STABLE_APPLE_ABI
    return isascii(c) ? __classic_upper_table()[c] : c;
#else
    return isascii(c) ? _DefaultRuneLocale.__mapupper[c] : c;
#endif
}

const char*
ctype<char>::do_toupper(char_type* low, const char_type* high) const
{
    for (; low != high; ++low)
#ifndef _LIBCPP_STABLE_APPLE_ABI
        *low = isascii(*low) ? __classic_upper_table()[*low] : *low;
#else
        *low = isascii(*low) ? _DefaultRuneLocale.__mapupper[*low] : *low;
#endif
    return low;
}

char
ctype<char>::do_tolower(char_type c) const
{
#ifndef _LIBCPP_STABLE_APPLE_ABI
    return isascii(c) ? __classic_lower_table()[c] : c;
#else
    return isascii(c) ? _DefaultRuneLocale.__maplower[c] : c;
#endif
}

const char*
ctype<char>::do_tolower(char_type* low, const char_type* high) const
{
    for (; low != high; ++low)
#ifndef _LIBCPP_STABLE_APPLE_ABI
        *low = isascii(*low) ? __classic_lower_table()[*low] : *low;
#else
        *low = isascii(*low) ? _DefaultRuneLocale.__maplower[*low] : *low;
#endif
    return low;
}

char
ctype<char>::do_widen(char c) const
{
    return c;
}

const char*
ctype<char>::do_widen(const char* low, const char* high, char_type* dest) const
{
    for (; low != high; ++low, ++dest)
        *dest = *low;
    return low;
}

char
ctype<char>::do_narrow(char_type c, char dfault) const
{
    if (isascii(c))
        return static_cast<char>(c);
    return dfault;
}

const char*
ctype<char>::do_narrow(const char_type* low, const char_type* high, char dfault, char* dest) const
{
    for (; low != high; ++low, ++dest)
        if (isascii(*low))
            *dest = *low;
        else
            *dest = dfault;
    return low;
}

const ctype<char>::mask*
ctype<char>::classic_table()  _NOEXCEPT
{
#ifdef __APPLE__
    return _DefaultRuneLocale.__runetype;
#elif defined(__GLIBC__)
    return __cloc()->__ctype_b;
// This is assumed to be safe.
#else
    return NULL;
#endif
}

#ifndef _LIBCPP_STABLE_APPLE_ABI
const int*
ctype<char>::__classic_lower_table() _NOEXCEPT
{
#ifdef __APPLE__
    return _DefaultRuneLocale.__maplower;
#elif defined(__GLIBC__)
    return __cloc()->__ctype_tolower;
#else
    return NULL;
#endif
}

const int*
ctype<char>::__classic_upper_table() _NOEXCEPT
{
#ifdef __APPLE__
    return _DefaultRuneLocale.__mapupper;
#elif defined(__GLIBC__)
    return __cloc()->__ctype_toupper;
#else
    return NULL;
#endif
}
#endif // _LIBCPP_STABLE_APPLE_ABI

// template <> class ctype_byname<char>

ctype_byname<char>::ctype_byname(const char* name, size_t refs)
    : ctype<char>(0, false, refs),
      __l(newlocale(LC_ALL_MASK, name, 0))
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__l == 0)
        throw runtime_error("ctype_byname<char>::ctype_byname"
                            " failed to construct for " + string(name));
#endif  // _LIBCPP_NO_EXCEPTIONS
}

ctype_byname<char>::ctype_byname(const string& name, size_t refs)
    : ctype<char>(0, false, refs),
      __l(newlocale(LC_ALL_MASK, name.c_str(), 0))
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__l == 0)
        throw runtime_error("ctype_byname<char>::ctype_byname"
                            " failed to construct for " + name);
#endif  // _LIBCPP_NO_EXCEPTIONS
}

ctype_byname<char>::~ctype_byname()
{
    freelocale(__l);
}

char
ctype_byname<char>::do_toupper(char_type c) const
{
    return toupper_l(c, __l);
}

const char*
ctype_byname<char>::do_toupper(char_type* low, const char_type* high) const
{
    for (; low != high; ++low)
        *low = toupper_l(*low, __l);
    return low;
}

char
ctype_byname<char>::do_tolower(char_type c) const
{
    return tolower_l(c, __l);
}

const char*
ctype_byname<char>::do_tolower(char_type* low, const char_type* high) const
{
    for (; low != high; ++low)
        *low = tolower_l(*low, __l);
    return low;
}

// template <> class ctype_byname<wchar_t>

ctype_byname<wchar_t>::ctype_byname(const char* name, size_t refs)
    : ctype<wchar_t>(refs),
      __l(newlocale(LC_ALL_MASK, name, 0))
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__l == 0)
        throw runtime_error("ctype_byname<wchar_t>::ctype_byname"
                            " failed to construct for " + string(name));
#endif  // _LIBCPP_NO_EXCEPTIONS
}

ctype_byname<wchar_t>::ctype_byname(const string& name, size_t refs)
    : ctype<wchar_t>(refs),
      __l(newlocale(LC_ALL_MASK, name.c_str(), 0))
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__l == 0)
        throw runtime_error("ctype_byname<wchar_t>::ctype_byname"
                            " failed to construct for " + name);
#endif  // _LIBCPP_NO_EXCEPTIONS
}

ctype_byname<wchar_t>::~ctype_byname()
{
    freelocale(__l);
}

bool
ctype_byname<wchar_t>::do_is(mask m, char_type c) const
{
#ifdef _LIBCPP_WCTYPE_IS_MASK
    return static_cast<bool>(iswctype_l(c, m, __l));
#else
    if (m & space && !iswspace_l(c, __l)) return false;
    if (m & print && !iswprint_l(c, __l)) return false;
    if (m & cntrl && !iswcntrl_l(c, __l)) return false;
    if (m & upper && !iswupper_l(c, __l)) return false;
    if (m & lower && !iswlower_l(c, __l)) return false;
    if (m & alpha && !iswalpha_l(c, __l)) return false;
    if (m & digit && !iswdigit_l(c, __l)) return false;
    if (m & punct && !iswpunct_l(c, __l)) return false;
    if (m & xdigit && !iswxdigit_l(c, __l)) return false;
    if (m & blank && !iswblank_l(c, __l)) return false;
    return true;
#endif
}

const wchar_t*
ctype_byname<wchar_t>::do_is(const char_type* low, const char_type* high, mask* vec) const
{
    for (; low != high; ++low, ++vec)
    {
        if (isascii(*low))
            *vec = static_cast<mask>(ctype<char>::classic_table()[*low]);
        else
        {
            *vec = 0;
            if (iswspace_l(*low, __l))
                *vec |= space;
            if (iswprint_l(*low, __l))
                *vec |= print;
            if (iswcntrl_l(*low, __l))
                *vec |= cntrl;
            if (iswupper_l(*low, __l))
                *vec |= upper;
            if (iswlower_l(*low, __l))
                *vec |= lower;
            if (iswalpha_l(*low, __l))
                *vec |= alpha;
            if (iswdigit_l(*low, __l))
                *vec |= digit;
            if (iswpunct_l(*low, __l))
                *vec |= punct;
            if (iswxdigit_l(*low, __l))
                *vec |= xdigit;
        }
    }
    return low;
}

const wchar_t*
ctype_byname<wchar_t>::do_scan_is(mask m, const char_type* low, const char_type* high) const
{
    for (; low != high; ++low)
    {
#ifdef _LIBCPP_WCTYPE_IS_MASK
        if (iswctype_l(*low, m, __l))
            break;
#else
        if (m & space && !iswspace_l(*low, __l)) continue;
        if (m & print && !iswprint_l(*low, __l)) continue;
        if (m & cntrl && !iswcntrl_l(*low, __l)) continue;
        if (m & upper && !iswupper_l(*low, __l)) continue;
        if (m & lower && !iswlower_l(*low, __l)) continue;
        if (m & alpha && !iswalpha_l(*low, __l)) continue;
        if (m & digit && !iswdigit_l(*low, __l)) continue;
        if (m & punct && !iswpunct_l(*low, __l)) continue;
        if (m & xdigit && !iswxdigit_l(*low, __l)) continue;
        if (m & blank && !iswblank_l(*low, __l)) continue;
        break;
#endif
    }
    return low;
}

const wchar_t*
ctype_byname<wchar_t>::do_scan_not(mask m, const char_type* low, const char_type* high) const
{
    for (; low != high; ++low)
    {
#ifdef _LIBCPP_WCTYPE_IS_MASK
        if (!iswctype_l(*low, m, __l))
            break;
#else
        if (m & space && iswspace_l(*low, __l)) continue;
        if (m & print && iswprint_l(*low, __l)) continue;
        if (m & cntrl && iswcntrl_l(*low, __l)) continue;
        if (m & upper && iswupper_l(*low, __l)) continue;
        if (m & lower && iswlower_l(*low, __l)) continue;
        if (m & alpha && iswalpha_l(*low, __l)) continue;
        if (m & digit && iswdigit_l(*low, __l)) continue;
        if (m & punct && iswpunct_l(*low, __l)) continue;
        if (m & xdigit && iswxdigit_l(*low, __l)) continue;
        if (m & blank && iswblank_l(*low, __l)) continue;
        break;
#endif
    }
    return low;
}

wchar_t
ctype_byname<wchar_t>::do_toupper(char_type c) const
{
    return towupper_l(c, __l);
}

const wchar_t*
ctype_byname<wchar_t>::do_toupper(char_type* low, const char_type* high) const
{
    for (; low != high; ++low)
        *low = towupper_l(*low, __l);
    return low;
}

wchar_t
ctype_byname<wchar_t>::do_tolower(char_type c) const
{
    return towlower_l(c, __l);
}

const wchar_t*
ctype_byname<wchar_t>::do_tolower(char_type* low, const char_type* high) const
{
    for (; low != high; ++low)
        *low = towlower_l(*low, __l);
    return low;
}

wchar_t
ctype_byname<wchar_t>::do_widen(char c) const
{
#ifdef _LIBCPP_STABLE_APPLE_ABI
    return btowc_l(c, __l);
#else
    return __btowc_l(c, __l);
#endif
}

const char*
ctype_byname<wchar_t>::do_widen(const char* low, const char* high, char_type* dest) const
{
    for (; low != high; ++low, ++dest)
#ifdef _LIBCPP_STABLE_APPLE_ABI
        *dest = btowc_l(*low, __l);
#else
        *dest = __btowc_l(*low, __l);
#endif
    return low;
}

char
ctype_byname<wchar_t>::do_narrow(char_type c, char dfault) const
{
#ifdef _LIBCPP_STABLE_APPLE_ABI
    int r = wctob_l(c, __l);
#else
    int r = __wctob_l(c, __l);
#endif
    return r != WEOF ? static_cast<char>(r) : dfault;
}

const wchar_t*
ctype_byname<wchar_t>::do_narrow(const char_type* low, const char_type* high, char dfault, char* dest) const
{
    for (; low != high; ++low, ++dest)
    {
#ifdef _LIBCPP_STABLE_APPLE_ABI
        int r = wctob_l(*low, __l);
#else
        int r = __wctob_l(*low, __l);
#endif
        *dest = r != WEOF ? static_cast<char>(r) : dfault;
    }
    return low;
}

// template <> class codecvt<char, char, mbstate_t>

locale::id codecvt<char, char, mbstate_t>::id;

codecvt<char, char, mbstate_t>::~codecvt()
{
}

codecvt<char, char, mbstate_t>::result
codecvt<char, char, mbstate_t>::do_out(state_type&,
    const intern_type* frm, const intern_type*, const intern_type*& frm_nxt,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    frm_nxt = frm;
    to_nxt = to;
    return noconv;
}

codecvt<char, char, mbstate_t>::result
codecvt<char, char, mbstate_t>::do_in(state_type&,
    const extern_type* frm, const extern_type*, const extern_type*& frm_nxt,
    intern_type* to, intern_type*, intern_type*& to_nxt) const
{
    frm_nxt = frm;
    to_nxt = to;
    return noconv;
}

codecvt<char, char, mbstate_t>::result
codecvt<char, char, mbstate_t>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
codecvt<char, char, mbstate_t>::do_encoding() const  _NOEXCEPT
{
    return 1;
}

bool
codecvt<char, char, mbstate_t>::do_always_noconv() const  _NOEXCEPT
{
    return true;
}

int
codecvt<char, char, mbstate_t>::do_length(state_type&,
    const extern_type* frm, const extern_type* end, size_t mx) const
{
    return static_cast<int>(min<size_t>(mx, end-frm));
}

int
codecvt<char, char, mbstate_t>::do_max_length() const  _NOEXCEPT
{
    return 1;
}

// template <> class codecvt<wchar_t, char, mbstate_t>

locale::id codecvt<wchar_t, char, mbstate_t>::id;

codecvt<wchar_t, char, mbstate_t>::codecvt(size_t refs)
    : locale::facet(refs),
      __l(0)
{
}

codecvt<wchar_t, char, mbstate_t>::codecvt(const char* nm, size_t refs)
    : locale::facet(refs),
      __l(newlocale(LC_ALL_MASK, nm, 0))
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__l == 0)
        throw runtime_error("codecvt_byname<wchar_t, char, mbstate_t>::codecvt_byname"
                            " failed to construct for " + string(nm));
#endif  // _LIBCPP_NO_EXCEPTIONS
}

codecvt<wchar_t, char, mbstate_t>::~codecvt()
{
    if (__l != 0)
        freelocale(__l);
}

codecvt<wchar_t, char, mbstate_t>::result
codecvt<wchar_t, char, mbstate_t>::do_out(state_type& st,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    // look for first internal null in frm
    const intern_type* fend = frm;
    for (; fend != frm_end; ++fend)
        if (*fend == 0)
            break;
    // loop over all null-terminated sequences in frm
    to_nxt = to;
    for (frm_nxt = frm; frm != frm_end && to != to_end; frm = frm_nxt, to = to_nxt)
    {
        // save state in case needed to reover to_nxt on error
        mbstate_t save_state = st;
#ifdef _LIBCPP_STABLE_APPLE_ABI
        size_t n = wcsnrtombs_l(to, &frm_nxt, fend-frm, to_end-to, &st, __l);
#else
        size_t n = __wcsnrtombs_l(to, &frm_nxt, fend-frm, to_end-to, &st, __l);
#endif
        if (n == size_t(-1))
        {
            // need to recover to_nxt
            for (to_nxt = to; frm != frm_nxt; ++frm)
            {
#ifdef _LIBCPP_STABLE_APPLE_ABI
                n = wcrtomb_l(to_nxt, *frm, &save_state, __l);
#else
                n = __wcrtomb_l(to_nxt, *frm, &save_state, __l);
#endif
                if (n == size_t(-1))
                    break;
                to_nxt += n;
            }
            frm_nxt = frm;
            return error;
        }
        if (n == 0)
            return partial;
        to_nxt += n;
        if (to_nxt == to_end)
            break;
        if (fend != frm_end)  // set up next null terminated sequence
        {
            // Try to write the terminating null
            extern_type tmp[MB_LEN_MAX];
#ifdef _LIBCPP_STABLE_APPLE_ABI
            n = wcrtomb_l(tmp, intern_type(), &st, __l);
#else
            n = __wcrtomb_l(tmp, intern_type(), &st, __l);
#endif
            if (n == size_t(-1))  // on error
                return error;
            if (n > to_end-to_nxt)  // is there room?
                return partial;
            for (extern_type* p = tmp; n; --n)  // write it
                *to_nxt++ = *p++;
            ++frm_nxt;
            // look for next null in frm
            for (fend = frm_nxt; fend != frm_end; ++fend)
                if (*fend == 0)
                    break;
        }
    }
    return frm_nxt == frm_end ? ok : partial;
}

codecvt<wchar_t, char, mbstate_t>::result
codecvt<wchar_t, char, mbstate_t>::do_in(state_type& st,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    // look for first internal null in frm
    const extern_type* fend = frm;
    for (; fend != frm_end; ++fend)
        if (*fend == 0)
            break;
    // loop over all null-terminated sequences in frm
    to_nxt = to;
    for (frm_nxt = frm; frm != frm_end && to != to_end; frm = frm_nxt, to = to_nxt)
    {
        // save state in case needed to reover to_nxt on error
        mbstate_t save_state = st;
#ifdef _LIBCPP_STABLE_APPLE_ABI
        size_t n = mbsnrtowcs_l(to, &frm_nxt, fend-frm, to_end-to, &st, __l);
#else
        size_t n = __mbsnrtowcs_l(to, &frm_nxt, fend-frm, to_end-to, &st, __l);
#endif
        if (n == size_t(-1))
        {
            // need to recover to_nxt
            for (to_nxt = to; frm != frm_nxt; ++to_nxt)
            {
#ifdef _LIBCPP_STABLE_APPLE_ABI
                n = mbrtowc_l(to_nxt, frm, fend-frm, &save_state, __l);
#else
                n = __mbrtowc_l(to_nxt, frm, fend-frm, &save_state, __l);
#endif
                switch (n)
                {
                case 0:
                    ++frm;
                    break;
                case -1:
                    frm_nxt = frm;
                    return error;
                case -2:
                    frm_nxt = frm;
                    return partial;
                default:
                    frm += n;
                    break;
                }
            }
            frm_nxt = frm;
            return frm_nxt == frm_end ? ok : partial;
        }
        if (n == 0)
            return error;
        to_nxt += n;
        if (to_nxt == to_end)
            break;
        if (fend != frm_end)  // set up next null terminated sequence
        {
            // Try to write the terminating null
#ifdef _LIBCPP_STABLE_APPLE_ABI
            n = mbrtowc_l(to_nxt, frm_nxt, 1, &st, __l);
#else
            n = __mbrtowc_l(to_nxt, frm_nxt, 1, &st, __l);
#endif
            if (n != 0)  // on error
                return error;
            ++to_nxt;
            ++frm_nxt;
            // look for next null in frm
            for (fend = frm_nxt; fend != frm_end; ++fend)
                if (*fend == 0)
                    break;
        }
    }
    return frm_nxt == frm_end ? ok : partial;
}

codecvt<wchar_t, char, mbstate_t>::result
codecvt<wchar_t, char, mbstate_t>::do_unshift(state_type& st,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    to_nxt = to;
    extern_type tmp[MB_LEN_MAX];
#ifdef _LIBCPP_STABLE_APPLE_ABI
    size_t n = wcrtomb_l(tmp, intern_type(), &st, __l);
#else
    size_t n = __wcrtomb_l(tmp, intern_type(), &st, __l);
#endif
    if (n == size_t(-1) || n == 0)  // on error
        return error;
    --n;
    if (n > to_end-to_nxt)  // is there room?
        return partial;
    for (extern_type* p = tmp; n; --n)  // write it
        *to_nxt++ = *p++;
    return ok;
}

int
codecvt<wchar_t, char, mbstate_t>::do_encoding() const  _NOEXCEPT
{
#ifdef _LIBCPP_STABLE_APPLE_ABI
    if (mbtowc_l((wchar_t*) 0, (const char*) 0, MB_LEN_MAX, __l) == 0)
#else
    if (__mbtowc_l((wchar_t*) 0, (const char*) 0, MB_LEN_MAX, __l) == 0)
#endif
    {
        // stateless encoding
#ifdef _LIBCPP_STABLE_APPLE_ABI
        if (__l == 0 || MB_CUR_MAX_L(__l) == 1)  // there are no known constant length encodings
#else
        if (__l == 0 || __mb_cur_max_l(__l) == 1)  // there are no known constant length encodings
#endif
            return 1;                // which take more than 1 char to form a wchar_t
         return 0;
    }
    return -1;
}

bool
codecvt<wchar_t, char, mbstate_t>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
codecvt<wchar_t, char, mbstate_t>::do_length(state_type& st,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    int nbytes = 0;
    for (size_t nwchar_t = 0; nwchar_t < mx && frm != frm_end; ++nwchar_t)
    {
#ifdef _LIBCPP_STABLE_APPLE_ABI
        size_t n = mbrlen_l(frm, frm_end-frm, &st, __l);
#else
        size_t n = __mbrlen_l(frm, frm_end-frm, &st, __l);
#endif
        switch (n)
        {
        case 0:
            ++nbytes;
            ++frm;
            break;
        case -1:
        case -2:
            return nbytes;
        default:
            nbytes += n;
            frm += n;
            break;
        }
    }
    return nbytes;
}

int
codecvt<wchar_t, char, mbstate_t>::do_max_length() const  _NOEXCEPT
{
#ifdef _LIBCPP_STABLE_APPLE_ABI
    return __l == 0 ? 1 : MB_CUR_MAX_L(__l);
#else
    return __l == 0 ? 1 : __mb_cur_max_l(__l);
#endif
}

//                                     Valid UTF ranges
//     UTF-32               UTF-16                          UTF-8               # of code points
//                     first      second       first   second    third   fourth
// 000000 - 00007F  0000 - 007F               00 - 7F                                 127
// 000080 - 0007FF  0080 - 07FF               C2 - DF, 80 - BF                       1920
// 000800 - 000FFF  0800 - 0FFF               E0 - E0, A0 - BF, 80 - BF              2048
// 001000 - 00CFFF  1000 - CFFF               E1 - EC, 80 - BF, 80 - BF             49152
// 00D000 - 00D7FF  D000 - D7FF               ED - ED, 80 - 9F, 80 - BF              2048
// 00D800 - 00DFFF                invalid
// 00E000 - 00FFFF  E000 - FFFF               EE - EF, 80 - BF, 80 - BF              8192
// 010000 - 03FFFF  D800 - D8BF, DC00 - DFFF  F0 - F0, 90 - BF, 80 - BF, 80 - BF   196608
// 040000 - 0FFFFF  D8C0 - DBBF, DC00 - DFFF  F1 - F3, 80 - BF, 80 - BF, 80 - BF   786432
// 100000 - 10FFFF  DBC0 - DBFF, DC00 - DFFF  F4 - F4, 80 - 8F, 80 - BF, 80 - BF    65536

static
codecvt_base::result
utf16_to_utf8(const uint16_t* frm, const uint16_t* frm_end, const uint16_t*& frm_nxt,
              uint8_t* to, uint8_t* to_end, uint8_t*& to_nxt,
              unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & generate_header)
    {
        if (to_end-to_nxt < 3)
            return codecvt_base::partial;
        *to_nxt++ = static_cast<uint8_t>(0xEF);
        *to_nxt++ = static_cast<uint8_t>(0xBB);
        *to_nxt++ = static_cast<uint8_t>(0xBF);
    }
    for (; frm_nxt < frm_end; ++frm_nxt)
    {
        uint16_t wc1 = *frm_nxt;
        if (wc1 > Maxcode)
            return codecvt_base::error;
        if (wc1 < 0x0080)
        {
            if (to_end-to_nxt < 1)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(wc1);
        }
        else if (wc1 < 0x0800)
        {
            if (to_end-to_nxt < 2)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(0xC0 | (wc1 >> 6));
            *to_nxt++ = static_cast<uint8_t>(0x80 | (wc1 & 0x03F));
        }
        else if (wc1 < 0xD800)
        {
            if (to_end-to_nxt < 3)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(0xE0 |  (wc1 >> 12));
            *to_nxt++ = static_cast<uint8_t>(0x80 | ((wc1 & 0x0FC0) >> 6));
            *to_nxt++ = static_cast<uint8_t>(0x80 |  (wc1 & 0x003F));
        }
        else if (wc1 < 0xDC00)
        {
            if (frm_end-frm_nxt < 2)
                return codecvt_base::partial;
            uint16_t wc2 = frm_nxt[1];
            if ((wc2 & 0xFC00) != 0xDC00)
                return codecvt_base::error;
            if (to_end-to_nxt < 4)
                return codecvt_base::partial;
            if ((((((unsigned long)wc1 & 0x03C0) >> 6) + 1) << 16) +
                (((unsigned long)wc1 & 0x003F) << 10) + (wc2 & 0x03FF) > Maxcode)
                return codecvt_base::error;
            ++frm_nxt;
            uint8_t z = ((wc1 & 0x03C0) >> 6) + 1;
            *to_nxt++ = static_cast<uint8_t>(0xF0 | (z >> 2));
            *to_nxt++ = static_cast<uint8_t>(0x80 | ((z & 0x03) << 4)     | ((wc1 & 0x003C) >> 2));
            *to_nxt++ = static_cast<uint8_t>(0x80 | ((wc1 & 0x0003) << 4) | ((wc2 & 0x03C0) >> 6));
            *to_nxt++ = static_cast<uint8_t>(0x80 |  (wc2 & 0x003F));
        }
        else if (wc1 < 0xE000)
        {
            return codecvt_base::error;
        }
        else
        {
            if (to_end-to_nxt < 3)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(0xE0 |  (wc1 >> 12));
            *to_nxt++ = static_cast<uint8_t>(0x80 | ((wc1 & 0x0FC0) >> 6));
            *to_nxt++ = static_cast<uint8_t>(0x80 |  (wc1 & 0x003F));
        }
    }
    return codecvt_base::ok;
}

static
codecvt_base::result
utf16_to_utf8(const uint32_t* frm, const uint32_t* frm_end, const uint32_t*& frm_nxt,
              uint8_t* to, uint8_t* to_end, uint8_t*& to_nxt,
              unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & generate_header)
    {
        if (to_end-to_nxt < 3)
            return codecvt_base::partial;
        *to_nxt++ = static_cast<uint8_t>(0xEF);
        *to_nxt++ = static_cast<uint8_t>(0xBB);
        *to_nxt++ = static_cast<uint8_t>(0xBF);
    }
    for (; frm_nxt < frm_end; ++frm_nxt)
    {
        uint16_t wc1 = static_cast<uint16_t>(*frm_nxt);
        if (wc1 > Maxcode)
            return codecvt_base::error;
        if (wc1 < 0x0080)
        {
            if (to_end-to_nxt < 1)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(wc1);
        }
        else if (wc1 < 0x0800)
        {
            if (to_end-to_nxt < 2)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(0xC0 | (wc1 >> 6));
            *to_nxt++ = static_cast<uint8_t>(0x80 | (wc1 & 0x03F));
        }
        else if (wc1 < 0xD800)
        {
            if (to_end-to_nxt < 3)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(0xE0 |  (wc1 >> 12));
            *to_nxt++ = static_cast<uint8_t>(0x80 | ((wc1 & 0x0FC0) >> 6));
            *to_nxt++ = static_cast<uint8_t>(0x80 |  (wc1 & 0x003F));
        }
        else if (wc1 < 0xDC00)
        {
            if (frm_end-frm_nxt < 2)
                return codecvt_base::partial;
            uint16_t wc2 = static_cast<uint16_t>(frm_nxt[1]);
            if ((wc2 & 0xFC00) != 0xDC00)
                return codecvt_base::error;
            if (to_end-to_nxt < 4)
                return codecvt_base::partial;
            if ((((((unsigned long)wc1 & 0x03C0) >> 6) + 1) << 16) +
                (((unsigned long)wc1 & 0x003F) << 10) + (wc2 & 0x03FF) > Maxcode)
                return codecvt_base::error;
            ++frm_nxt;
            uint8_t z = ((wc1 & 0x03C0) >> 6) + 1;
            *to_nxt++ = static_cast<uint8_t>(0xF0 | (z >> 2));
            *to_nxt++ = static_cast<uint8_t>(0x80 | ((z & 0x03) << 4)     | ((wc1 & 0x003C) >> 2));
            *to_nxt++ = static_cast<uint8_t>(0x80 | ((wc1 & 0x0003) << 4) | ((wc2 & 0x03C0) >> 6));
            *to_nxt++ = static_cast<uint8_t>(0x80 |  (wc2 & 0x003F));
        }
        else if (wc1 < 0xE000)
        {
            return codecvt_base::error;
        }
        else
        {
            if (to_end-to_nxt < 3)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(0xE0 |  (wc1 >> 12));
            *to_nxt++ = static_cast<uint8_t>(0x80 | ((wc1 & 0x0FC0) >> 6));
            *to_nxt++ = static_cast<uint8_t>(0x80 |  (wc1 & 0x003F));
        }
    }
    return codecvt_base::ok;
}

static
codecvt_base::result
utf8_to_utf16(const uint8_t* frm, const uint8_t* frm_end, const uint8_t*& frm_nxt,
              uint16_t* to, uint16_t* to_end, uint16_t*& to_nxt,
              unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 3 && frm_nxt[0] == 0xEF && frm_nxt[1] == 0xBB &&
                                                          frm_nxt[2] == 0xBF)
            frm_nxt += 3;
    }
    for (; frm_nxt < frm_end && to_nxt < to_end; ++to_nxt)
    {
        uint8_t c1 = *frm_nxt;
        if (c1 > Maxcode)
            return codecvt_base::error;
        if (c1 < 0x80)
        {
            *to_nxt = static_cast<uint16_t>(c1);
            ++frm_nxt;
        }
        else if (c1 < 0xC2)
        {
            return codecvt_base::error;
        }
        else if (c1 < 0xE0)
        {
            if (frm_end-frm_nxt < 2)
                return codecvt_base::partial;
            uint8_t c2 = frm_nxt[1];
            if ((c2 & 0xC0) != 0x80)
                return codecvt_base::error;
            uint16_t t = static_cast<uint16_t>(((c1 & 0x1F) << 6) | (c2 & 0x3F));
            if (t > Maxcode)
                return codecvt_base::error;
            *to_nxt = t;
            frm_nxt += 2;
        }
        else if (c1 < 0xF0)
        {
            if (frm_end-frm_nxt < 3)
                return codecvt_base::partial;
            uint8_t c2 = frm_nxt[1];
            uint8_t c3 = frm_nxt[2];
            switch (c1)
            {
            case 0xE0:
                if ((c2 & 0xE0) != 0xA0)
                    return codecvt_base::error;
                 break;
            case 0xED:
                if ((c2 & 0xE0) != 0x80)
                    return codecvt_base::error;
                 break;
            default:
                if ((c2 & 0xC0) != 0x80)
                    return codecvt_base::error;
                 break;
            }
            if ((c3 & 0xC0) != 0x80)
                return codecvt_base::error;
            uint16_t t = static_cast<uint16_t>(((c1 & 0x0F) << 12)
                                             | ((c2 & 0x3F) << 6)
                                             |  (c3 & 0x3F));
            if (t > Maxcode)
                return codecvt_base::error;
            *to_nxt = t;
            frm_nxt += 3;
        }
        else if (c1 < 0xF5)
        {
            if (frm_end-frm_nxt < 4)
                return codecvt_base::partial;
            uint8_t c2 = frm_nxt[1];
            uint8_t c3 = frm_nxt[2];
            uint8_t c4 = frm_nxt[3];
            switch (c1)
            {
            case 0xF0:
                if (!(0x90 <= c2 && c2 <= 0xBF))
                    return codecvt_base::error;
                 break;
            case 0xF4:
                if ((c2 & 0xF0) != 0x80)
                    return codecvt_base::error;
                 break;
            default:
                if ((c2 & 0xC0) != 0x80)
                    return codecvt_base::error;
                 break;
            }
            if ((c3 & 0xC0) != 0x80 || (c4 & 0xC0) != 0x80)
                return codecvt_base::error;
            if (to_end-to_nxt < 2)
                return codecvt_base::partial;
            if (((((unsigned long)c1 & 7) << 18) +
                (((unsigned long)c2 & 0x3F) << 12) +
                (((unsigned long)c3 & 0x3F) << 6) + (c4 & 0x3F)) > Maxcode)
                return codecvt_base::error;
            *to_nxt = static_cast<uint16_t>(
                    0xD800
                  | (((((c1 & 0x07) << 2) | ((c2 & 0x30) >> 4)) - 1) << 6)
                  | ((c2 & 0x0F) << 2)
                  | ((c3 & 0x30) >> 4));
            *++to_nxt = static_cast<uint16_t>(
                    0xDC00
                  | ((c3 & 0x0F) << 6)
                  |  (c4 & 0x3F));
            frm_nxt += 4;
        }
        else
        {
            return codecvt_base::error;
        }
    }
    return frm_nxt < frm_end ? codecvt_base::partial : codecvt_base::ok;
}

static
codecvt_base::result
utf8_to_utf16(const uint8_t* frm, const uint8_t* frm_end, const uint8_t*& frm_nxt,
              uint32_t* to, uint32_t* to_end, uint32_t*& to_nxt,
              unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 3 && frm_nxt[0] == 0xEF && frm_nxt[1] == 0xBB &&
                                                          frm_nxt[2] == 0xBF)
            frm_nxt += 3;
    }
    for (; frm_nxt < frm_end && to_nxt < to_end; ++to_nxt)
    {
        uint8_t c1 = *frm_nxt;
        if (c1 > Maxcode)
            return codecvt_base::error;
        if (c1 < 0x80)
        {
            *to_nxt = static_cast<uint32_t>(c1);
            ++frm_nxt;
        }
        else if (c1 < 0xC2)
        {
            return codecvt_base::error;
        }
        else if (c1 < 0xE0)
        {
            if (frm_end-frm_nxt < 2)
                return codecvt_base::partial;
            uint8_t c2 = frm_nxt[1];
            if ((c2 & 0xC0) != 0x80)
                return codecvt_base::error;
            uint16_t t = static_cast<uint16_t>(((c1 & 0x1F) << 6) | (c2 & 0x3F));
            if (t > Maxcode)
                return codecvt_base::error;
            *to_nxt = static_cast<uint32_t>(t);
            frm_nxt += 2;
        }
        else if (c1 < 0xF0)
        {
            if (frm_end-frm_nxt < 3)
                return codecvt_base::partial;
            uint8_t c2 = frm_nxt[1];
            uint8_t c3 = frm_nxt[2];
            switch (c1)
            {
            case 0xE0:
                if ((c2 & 0xE0) != 0xA0)
                    return codecvt_base::error;
                 break;
            case 0xED:
                if ((c2 & 0xE0) != 0x80)
                    return codecvt_base::error;
                 break;
            default:
                if ((c2 & 0xC0) != 0x80)
                    return codecvt_base::error;
                 break;
            }
            if ((c3 & 0xC0) != 0x80)
                return codecvt_base::error;
            uint16_t t = static_cast<uint16_t>(((c1 & 0x0F) << 12)
                                             | ((c2 & 0x3F) << 6)
                                             |  (c3 & 0x3F));
            if (t > Maxcode)
                return codecvt_base::error;
            *to_nxt = static_cast<uint32_t>(t);
            frm_nxt += 3;
        }
        else if (c1 < 0xF5)
        {
            if (frm_end-frm_nxt < 4)
                return codecvt_base::partial;
            uint8_t c2 = frm_nxt[1];
            uint8_t c3 = frm_nxt[2];
            uint8_t c4 = frm_nxt[3];
            switch (c1)
            {
            case 0xF0:
                if (!(0x90 <= c2 && c2 <= 0xBF))
                    return codecvt_base::error;
                 break;
            case 0xF4:
                if ((c2 & 0xF0) != 0x80)
                    return codecvt_base::error;
                 break;
            default:
                if ((c2 & 0xC0) != 0x80)
                    return codecvt_base::error;
                 break;
            }
            if ((c3 & 0xC0) != 0x80 || (c4 & 0xC0) != 0x80)
                return codecvt_base::error;
            if (to_end-to_nxt < 2)
                return codecvt_base::partial;
            if (((((unsigned long)c1 & 7) << 18) +
                (((unsigned long)c2 & 0x3F) << 12) +
                (((unsigned long)c3 & 0x3F) << 6) + (c4 & 0x3F)) > Maxcode)
                return codecvt_base::error;
            *to_nxt = static_cast<uint32_t>(
                    0xD800
                  | (((((c1 & 0x07) << 2) | ((c2 & 0x30) >> 4)) - 1) << 6)
                  | ((c2 & 0x0F) << 2)
                  | ((c3 & 0x30) >> 4));
            *++to_nxt = static_cast<uint32_t>(
                    0xDC00
                  | ((c3 & 0x0F) << 6)
                  |  (c4 & 0x3F));
            frm_nxt += 4;
        }
        else
        {
            return codecvt_base::error;
        }
    }
    return frm_nxt < frm_end ? codecvt_base::partial : codecvt_base::ok;
}

static
int
utf8_to_utf16_length(const uint8_t* frm, const uint8_t* frm_end,
                     size_t mx, unsigned long Maxcode = 0x10FFFF,
                     codecvt_mode mode = codecvt_mode(0))
{
    const uint8_t* frm_nxt = frm;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 3 && frm_nxt[0] == 0xEF && frm_nxt[1] == 0xBB &&
                                                          frm_nxt[2] == 0xBF)
            frm_nxt += 3;
    }
    for (size_t nchar16_t = 0; frm_nxt < frm_end && nchar16_t < mx; ++nchar16_t)
    {
        uint8_t c1 = *frm_nxt;
        if (c1 > Maxcode)
            break;
        if (c1 < 0x80)
        {
            ++frm_nxt;
        }
        else if (c1 < 0xC2)
        {
            break;
        }
        else if (c1 < 0xE0)
        {
            if ((frm_end-frm_nxt < 2) || (frm_nxt[1] & 0xC0) != 0x80)
                break;
            uint16_t t = static_cast<uint16_t>(((c1 & 0x1F) << 6) | (frm_nxt[1] & 0x3F));
            if (t > Maxcode)
                break;
            frm_nxt += 2;
        }
        else if (c1 < 0xF0)
        {
            if (frm_end-frm_nxt < 3)
                break;
            uint8_t c2 = frm_nxt[1];
            uint8_t c3 = frm_nxt[2];
            uint16_t t = static_cast<uint16_t>(((c1 & 0x0F) << 12)
                                             | ((c2 & 0x3F) << 6)
                                             |  (c3 & 0x3F));
            switch (c1)
            {
            case 0xE0:
                if ((c2 & 0xE0) != 0xA0)
                    return static_cast<int>(frm_nxt - frm);
                break;
            case 0xED:
                if ((c2 & 0xE0) != 0x80)
                    return static_cast<int>(frm_nxt - frm);
                 break;
            default:
                if ((c2 & 0xC0) != 0x80)
                    return static_cast<int>(frm_nxt - frm);
                 break;
            }
            if ((c3 & 0xC0) != 0x80)
                break;
            if ((((c1 & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F)) > Maxcode)
                break;
            frm_nxt += 3;
        }
        else if (c1 < 0xF5)
        {
            if (frm_end-frm_nxt < 4 || mx-nchar16_t < 2)
                break;
            uint8_t c2 = frm_nxt[1];
            uint8_t c3 = frm_nxt[2];
            uint8_t c4 = frm_nxt[3];
            switch (c1)
            {
            case 0xF0:
                if (!(0x90 <= c2 && c2 <= 0xBF))
                    return static_cast<int>(frm_nxt - frm);
                 break;
            case 0xF4:
                if ((c2 & 0xF0) != 0x80)
                    return static_cast<int>(frm_nxt - frm);
                 break;
            default:
                if ((c2 & 0xC0) != 0x80)
                    return static_cast<int>(frm_nxt - frm);
                 break;
            }
            if ((c3 & 0xC0) != 0x80 || (c4 & 0xC0) != 0x80)
                break;
            if (((((unsigned long)c1 & 7) << 18) +
                (((unsigned long)c2 & 0x3F) << 12) +
                (((unsigned long)c3 & 0x3F) << 6) + (c4 & 0x3F)) > Maxcode)
                break;
            ++nchar16_t;
            frm_nxt += 4;
        }
        else
        {
            break;
        }
    }
    return static_cast<int>(frm_nxt - frm);
}

static
codecvt_base::result
ucs4_to_utf8(const uint32_t* frm, const uint32_t* frm_end, const uint32_t*& frm_nxt,
             uint8_t* to, uint8_t* to_end, uint8_t*& to_nxt,
             unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & generate_header)
    {
        if (to_end-to_nxt < 3)
            return codecvt_base::partial;
        *to_nxt++ = static_cast<uint8_t>(0xEF);
        *to_nxt++ = static_cast<uint8_t>(0xBB);
        *to_nxt++ = static_cast<uint8_t>(0xBF);
    }
    for (; frm_nxt < frm_end; ++frm_nxt)
    {
        uint32_t wc = *frm_nxt;
        if ((wc & 0xFFFFF800) == 0x00D800 || wc > Maxcode)
            return codecvt_base::error;
        if (wc < 0x000080)
        {
            if (to_end-to_nxt < 1)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(wc);
        }
        else if (wc < 0x000800)
        {
            if (to_end-to_nxt < 2)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(0xC0 | (wc >> 6));
            *to_nxt++ = static_cast<uint8_t>(0x80 | (wc & 0x03F));
        }
        else if (wc < 0x010000)
        {
            if (to_end-to_nxt < 3)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(0xE0 |  (wc >> 12));
            *to_nxt++ = static_cast<uint8_t>(0x80 | ((wc & 0x0FC0) >> 6));
            *to_nxt++ = static_cast<uint8_t>(0x80 |  (wc & 0x003F));
        }
        else // if (wc < 0x110000)
        {
            if (to_end-to_nxt < 4)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(0xF0 |  (wc >> 18));
            *to_nxt++ = static_cast<uint8_t>(0x80 | ((wc & 0x03F000) >> 12));
            *to_nxt++ = static_cast<uint8_t>(0x80 | ((wc & 0x000FC0) >> 6));
            *to_nxt++ = static_cast<uint8_t>(0x80 |  (wc & 0x00003F));
        }
    }
    return codecvt_base::ok;
}

static
codecvt_base::result
utf8_to_ucs4(const uint8_t* frm, const uint8_t* frm_end, const uint8_t*& frm_nxt,
             uint32_t* to, uint32_t* to_end, uint32_t*& to_nxt,
             unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 3 && frm_nxt[0] == 0xEF && frm_nxt[1] == 0xBB &&
                                                          frm_nxt[2] == 0xBF)
            frm_nxt += 3;
    }
    for (; frm_nxt < frm_end && to_nxt < to_end; ++to_nxt)
    {
        uint8_t c1 = static_cast<uint8_t>(*frm_nxt);
        if (c1 < 0x80)
        {
            if (c1 > Maxcode)
                return codecvt_base::error;
            *to_nxt = static_cast<uint32_t>(c1);
            ++frm_nxt;
        }
        else if (c1 < 0xC2)
        {
            return codecvt_base::error;
        }
        else if (c1 < 0xE0)
        {
            if (frm_end-frm_nxt < 2)
                return codecvt_base::partial;
            uint8_t c2 = frm_nxt[1];
            if ((c2 & 0xC0) != 0x80)
                return codecvt_base::error;
            uint32_t t = static_cast<uint32_t>(((c1 & 0x1F) << 6)
                                              | (c2 & 0x3F));
            if (t > Maxcode)
                return codecvt_base::error;
            *to_nxt = t;
            frm_nxt += 2;
        }
        else if (c1 < 0xF0)
        {
            if (frm_end-frm_nxt < 3)
                return codecvt_base::partial;
            uint8_t c2 = frm_nxt[1];
            uint8_t c3 = frm_nxt[2];
            switch (c1)
            {
            case 0xE0:
                if ((c2 & 0xE0) != 0xA0)
                    return codecvt_base::error;
                 break;
            case 0xED:
                if ((c2 & 0xE0) != 0x80)
                    return codecvt_base::error;
                 break;
            default:
                if ((c2 & 0xC0) != 0x80)
                    return codecvt_base::error;
                 break;
            }
            if ((c3 & 0xC0) != 0x80)
                return codecvt_base::error;
            uint32_t t = static_cast<uint32_t>(((c1 & 0x0F) << 12)
                                             | ((c2 & 0x3F) << 6)
                                             |  (c3 & 0x3F));
            if (t > Maxcode)
                return codecvt_base::error;
            *to_nxt = t;
            frm_nxt += 3;
        }
        else if (c1 < 0xF5)
        {
            if (frm_end-frm_nxt < 4)
                return codecvt_base::partial;
            uint8_t c2 = frm_nxt[1];
            uint8_t c3 = frm_nxt[2];
            uint8_t c4 = frm_nxt[3];
            switch (c1)
            {
            case 0xF0:
                if (!(0x90 <= c2 && c2 <= 0xBF))
                    return codecvt_base::error;
                 break;
            case 0xF4:
                if ((c2 & 0xF0) != 0x80)
                    return codecvt_base::error;
                 break;
            default:
                if ((c2 & 0xC0) != 0x80)
                    return codecvt_base::error;
                 break;
            }
            if ((c3 & 0xC0) != 0x80 || (c4 & 0xC0) != 0x80)
                return codecvt_base::error;
            uint32_t t = static_cast<uint32_t>(((c1 & 0x07) << 18)
                                             | ((c2 & 0x3F) << 12)
                                             | ((c3 & 0x3F) << 6)
                                             |  (c4 & 0x3F));
            if (t > Maxcode)
                return codecvt_base::error;
            *to_nxt = t;
            frm_nxt += 4;
        }
        else
        {
            return codecvt_base::error;
        }
    }
    return frm_nxt < frm_end ? codecvt_base::partial : codecvt_base::ok;
}

static
int
utf8_to_ucs4_length(const uint8_t* frm, const uint8_t* frm_end,
                    size_t mx, unsigned long Maxcode = 0x10FFFF,
                    codecvt_mode mode = codecvt_mode(0))
{
    const uint8_t* frm_nxt = frm;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 3 && frm_nxt[0] == 0xEF && frm_nxt[1] == 0xBB &&
                                                          frm_nxt[2] == 0xBF)
            frm_nxt += 3;
    }
    for (size_t nchar32_t = 0; frm_nxt < frm_end && nchar32_t < mx; ++nchar32_t)
    {
        uint8_t c1 = static_cast<uint8_t>(*frm_nxt);
        if (c1 < 0x80)
        {
            if (c1 > Maxcode)
                break;
            ++frm_nxt;
        }
        else if (c1 < 0xC2)
        {
            break;
        }
        else if (c1 < 0xE0)
        {
            if ((frm_end-frm_nxt < 2) || ((frm_nxt[1] & 0xC0) != 0x80))
                break;
            if ((((c1 & 0x1F) << 6) | (frm_nxt[1] & 0x3F)) > Maxcode)
                break;
            frm_nxt += 2;
        }
        else if (c1 < 0xF0)
        {
            if (frm_end-frm_nxt < 3)
                break;
            uint8_t c2 = frm_nxt[1];
            uint8_t c3 = frm_nxt[2];
            switch (c1)
            {
            case 0xE0:
                if ((c2 & 0xE0) != 0xA0)
                    return static_cast<int>(frm_nxt - frm);
                break;
            case 0xED:
                if ((c2 & 0xE0) != 0x80)
                    return static_cast<int>(frm_nxt - frm);
                 break;
            default:
                if ((c2 & 0xC0) != 0x80)
                    return static_cast<int>(frm_nxt - frm);
                 break;
            }
            if ((c3 & 0xC0) != 0x80)
                break;
            if ((((c1 & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F)) > Maxcode)
                break;
            frm_nxt += 3;
        }
        else if (c1 < 0xF5)
        {
            if (frm_end-frm_nxt < 4)
                break;
            uint8_t c2 = frm_nxt[1];
            uint8_t c3 = frm_nxt[2];
            uint8_t c4 = frm_nxt[3];
            switch (c1)
            {
            case 0xF0:
                if (!(0x90 <= c2 && c2 <= 0xBF))
                    return static_cast<int>(frm_nxt - frm);
                 break;
            case 0xF4:
                if ((c2 & 0xF0) != 0x80)
                    return static_cast<int>(frm_nxt - frm);
                 break;
            default:
                if ((c2 & 0xC0) != 0x80)
                    return static_cast<int>(frm_nxt - frm);
                 break;
            }
            if ((c3 & 0xC0) != 0x80 || (c4 & 0xC0) != 0x80)
                break;
            uint32_t t = static_cast<uint32_t>(((c1 & 0x07) << 18)
                                             | ((c2 & 0x3F) << 12)
                                             | ((c3 & 0x3F) << 6)
                                             |  (c4 & 0x3F));
            if ((((c1 & 0x07) << 18) | ((c2 & 0x3F) << 12) |
                 ((c3 & 0x3F) << 6)  |  (c4 & 0x3F)) > Maxcode)
                break;
            frm_nxt += 4;
        }
        else
        {
            break;
        }
    }
    return static_cast<int>(frm_nxt - frm);
}

static
codecvt_base::result
ucs2_to_utf8(const uint16_t* frm, const uint16_t* frm_end, const uint16_t*& frm_nxt,
             uint8_t* to, uint8_t* to_end, uint8_t*& to_nxt,
             unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & generate_header)
    {
        if (to_end-to_nxt < 3)
            return codecvt_base::partial;
        *to_nxt++ = static_cast<uint8_t>(0xEF);
        *to_nxt++ = static_cast<uint8_t>(0xBB);
        *to_nxt++ = static_cast<uint8_t>(0xBF);
    }
    for (; frm_nxt < frm_end; ++frm_nxt)
    {
        uint16_t wc = *frm_nxt;
        if ((wc & 0xF800) == 0xD800 || wc > Maxcode)
            return codecvt_base::error;
        if (wc < 0x0080)
        {
            if (to_end-to_nxt < 1)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(wc);
        }
        else if (wc < 0x0800)
        {
            if (to_end-to_nxt < 2)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(0xC0 | (wc >> 6));
            *to_nxt++ = static_cast<uint8_t>(0x80 | (wc & 0x03F));
        }
        else // if (wc <= 0xFFFF)
        {
            if (to_end-to_nxt < 3)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(0xE0 |  (wc >> 12));
            *to_nxt++ = static_cast<uint8_t>(0x80 | ((wc & 0x0FC0) >> 6));
            *to_nxt++ = static_cast<uint8_t>(0x80 |  (wc & 0x003F));
        }
    }
    return codecvt_base::ok;
}

static
codecvt_base::result
utf8_to_ucs2(const uint8_t* frm, const uint8_t* frm_end, const uint8_t*& frm_nxt,
             uint16_t* to, uint16_t* to_end, uint16_t*& to_nxt,
             unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 3 && frm_nxt[0] == 0xEF && frm_nxt[1] == 0xBB &&
                                                          frm_nxt[2] == 0xBF)
            frm_nxt += 3;
    }
    for (; frm_nxt < frm_end && to_nxt < to_end; ++to_nxt)
    {
        uint8_t c1 = static_cast<uint8_t>(*frm_nxt);
        if (c1 < 0x80)
        {
            if (c1 > Maxcode)
                return codecvt_base::error;
            *to_nxt = static_cast<uint16_t>(c1);
            ++frm_nxt;
        }
        else if (c1 < 0xC2)
        {
            return codecvt_base::error;
        }
        else if (c1 < 0xE0)
        {
            if (frm_end-frm_nxt < 2)
                return codecvt_base::partial;
            uint8_t c2 = frm_nxt[1];
            if ((c2 & 0xC0) != 0x80)
                return codecvt_base::error;
            uint16_t t = static_cast<uint16_t>(((c1 & 0x1F) << 6)
                                              | (c2 & 0x3F));
            if (t > Maxcode)
                return codecvt_base::error;
            *to_nxt = t;
            frm_nxt += 2;
        }
        else if (c1 < 0xF0)
        {
            if (frm_end-frm_nxt < 3)
                return codecvt_base::partial;
            uint8_t c2 = frm_nxt[1];
            uint8_t c3 = frm_nxt[2];
            switch (c1)
            {
            case 0xE0:
                if ((c2 & 0xE0) != 0xA0)
                    return codecvt_base::error;
                 break;
            case 0xED:
                if ((c2 & 0xE0) != 0x80)
                    return codecvt_base::error;
                 break;
            default:
                if ((c2 & 0xC0) != 0x80)
                    return codecvt_base::error;
                 break;
            }
            if ((c3 & 0xC0) != 0x80)
                return codecvt_base::error;
            uint16_t t = static_cast<uint16_t>(((c1 & 0x0F) << 12)
                                             | ((c2 & 0x3F) << 6)
                                             |  (c3 & 0x3F));
            if (t > Maxcode)
                return codecvt_base::error;
            *to_nxt = t;
            frm_nxt += 3;
        }
        else
        {
            return codecvt_base::error;
        }
    }
    return frm_nxt < frm_end ? codecvt_base::partial : codecvt_base::ok;
}

static
int
utf8_to_ucs2_length(const uint8_t* frm, const uint8_t* frm_end,
                    size_t mx, unsigned long Maxcode = 0x10FFFF,
                    codecvt_mode mode = codecvt_mode(0))
{
    const uint8_t* frm_nxt = frm;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 3 && frm_nxt[0] == 0xEF && frm_nxt[1] == 0xBB &&
                                                          frm_nxt[2] == 0xBF)
            frm_nxt += 3;
    }
    for (size_t nchar32_t = 0; frm_nxt < frm_end && nchar32_t < mx; ++nchar32_t)
    {
        uint8_t c1 = static_cast<uint8_t>(*frm_nxt);
        if (c1 < 0x80)
        {
            if (c1 > Maxcode)
                break;
            ++frm_nxt;
        }
        else if (c1 < 0xC2)
        {
            break;
        }
        else if (c1 < 0xE0)
        {
            if ((frm_end-frm_nxt < 2) || ((frm_nxt[1] & 0xC0) != 0x80))
                break;
            if ((((c1 & 0x1F) << 6) | (frm_nxt[1] & 0x3F)) > Maxcode)
                break;
            frm_nxt += 2;
        }
        else if (c1 < 0xF0)
        {
            if (frm_end-frm_nxt < 3)
                break;
            uint8_t c2 = frm_nxt[1];
            uint8_t c3 = frm_nxt[2];
            switch (c1)
            {
            case 0xE0:
                if ((c2 & 0xE0) != 0xA0)
                    return static_cast<int>(frm_nxt - frm);
                break;
            case 0xED:
                if ((c2 & 0xE0) != 0x80)
                    return static_cast<int>(frm_nxt - frm);
                 break;
            default:
                if ((c2 & 0xC0) != 0x80)
                    return static_cast<int>(frm_nxt - frm);
                 break;
            }
            if ((c3 & 0xC0) != 0x80)
                break;
            if ((((c1 & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F)) > Maxcode)
                break;
            frm_nxt += 3;
        }
        else
        {
            break;
        }
    }
    return static_cast<int>(frm_nxt - frm);
}

static
codecvt_base::result
ucs4_to_utf16be(const uint32_t* frm, const uint32_t* frm_end, const uint32_t*& frm_nxt,
                uint8_t* to, uint8_t* to_end, uint8_t*& to_nxt,
                unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & generate_header)
    {
        if (to_end-to_nxt < 2)
            return codecvt_base::partial;
        *to_nxt++ = static_cast<uint8_t>(0xFE);
        *to_nxt++ = static_cast<uint8_t>(0xFF);
    }
    for (; frm_nxt < frm_end; ++frm_nxt)
    {
        uint32_t wc = *frm_nxt;
        if ((wc & 0xFFFFF800) == 0x00D800 || wc > Maxcode)
            return codecvt_base::error;
        if (wc < 0x010000)
        {
            if (to_end-to_nxt < 2)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(wc >> 8);
            *to_nxt++ = static_cast<uint8_t>(wc);
        }
        else
        {
            if (to_end-to_nxt < 4)
                return codecvt_base::partial;
            uint16_t t = static_cast<uint16_t>(
                    0xD800
                  | ((((wc & 0x1F0000) >> 16) - 1) << 6)
                  |   ((wc & 0x00FC00) >> 10));
            *to_nxt++ = static_cast<uint8_t>(t >> 8);
            *to_nxt++ = static_cast<uint8_t>(t);
            t = static_cast<uint16_t>(0xDC00 | (wc & 0x03FF));
            *to_nxt++ = static_cast<uint8_t>(t >> 8);
            *to_nxt++ = static_cast<uint8_t>(t);
        }
    }
    return codecvt_base::ok;
}

static
codecvt_base::result
utf16be_to_ucs4(const uint8_t* frm, const uint8_t* frm_end, const uint8_t*& frm_nxt,
                uint32_t* to, uint32_t* to_end, uint32_t*& to_nxt,
                unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 2 && frm_nxt[0] == 0xFE && frm_nxt[1] == 0xFF)
            frm_nxt += 2;
    }
    for (; frm_nxt < frm_end - 1 && to_nxt < to_end; ++to_nxt)
    {
        uint16_t c1 = frm_nxt[0] << 8 | frm_nxt[1];
        if ((c1 & 0xFC00) == 0xDC00)
            return codecvt_base::error;
        if ((c1 & 0xFC00) != 0xD800)
        {
            if (c1 > Maxcode)
                return codecvt_base::error;
            *to_nxt = static_cast<uint32_t>(c1);
            frm_nxt += 2;
        }
        else
        {
            if (frm_end-frm_nxt < 4)
                return codecvt_base::partial;
            uint16_t c2 = frm_nxt[2] << 8 | frm_nxt[3];
            if ((c2 & 0xFC00) != 0xDC00)
                return codecvt_base::error;
            uint32_t t = static_cast<uint32_t>(
                    ((((c1 & 0x03C0) >> 6) + 1) << 16)
                  |   ((c1 & 0x003F) << 10)
                  |    (c2 & 0x03FF));
            if (t > Maxcode)
                return codecvt_base::error;
            *to_nxt = t;
            frm_nxt += 4;
        }
    }
    return frm_nxt < frm_end ? codecvt_base::partial : codecvt_base::ok;
}

static
int
utf16be_to_ucs4_length(const uint8_t* frm, const uint8_t* frm_end,
                       size_t mx, unsigned long Maxcode = 0x10FFFF,
                       codecvt_mode mode = codecvt_mode(0))
{
    const uint8_t* frm_nxt = frm;
    frm_nxt = frm;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 2 && frm_nxt[0] == 0xFE && frm_nxt[1] == 0xFF)
            frm_nxt += 2;
    }
    for (size_t nchar32_t = 0; frm_nxt < frm_end - 1 && nchar32_t < mx; ++nchar32_t)
    {
        uint16_t c1 = frm_nxt[0] << 8 | frm_nxt[1];
        if ((c1 & 0xFC00) == 0xDC00)
            break;
        if ((c1 & 0xFC00) != 0xD800)
        {
            if (c1 > Maxcode)
                break;
            frm_nxt += 2;
        }
        else
        {
            if (frm_end-frm_nxt < 4)
                break;
            uint16_t c2 = frm_nxt[2] << 8 | frm_nxt[3];
            if ((c2 & 0xFC00) != 0xDC00)
                break;
            uint32_t t = static_cast<uint32_t>(
                    ((((c1 & 0x03C0) >> 6) + 1) << 16)
                  |   ((c1 & 0x003F) << 10)
                  |    (c2 & 0x03FF));
            if (t > Maxcode)
                break;
            frm_nxt += 4;
        }
    }
    return static_cast<int>(frm_nxt - frm);
}

static
codecvt_base::result
ucs4_to_utf16le(const uint32_t* frm, const uint32_t* frm_end, const uint32_t*& frm_nxt,
                uint8_t* to, uint8_t* to_end, uint8_t*& to_nxt,
                unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & generate_header)
    {
        if (to_end-to_nxt < 2)
            return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(0xFF);
            *to_nxt++ = static_cast<uint8_t>(0xFE);
    }
    for (; frm_nxt < frm_end; ++frm_nxt)
    {
        uint32_t wc = *frm_nxt;
        if ((wc & 0xFFFFF800) == 0x00D800 || wc > Maxcode)
            return codecvt_base::error;
        if (wc < 0x010000)
        {
            if (to_end-to_nxt < 2)
                return codecvt_base::partial;
            *to_nxt++ = static_cast<uint8_t>(wc);
            *to_nxt++ = static_cast<uint8_t>(wc >> 8);
        }
        else
        {
            if (to_end-to_nxt < 4)
                return codecvt_base::partial;
            uint16_t t = static_cast<uint16_t>(
                    0xD800
                  | ((((wc & 0x1F0000) >> 16) - 1) << 6)
                  |   ((wc & 0x00FC00) >> 10));
            *to_nxt++ = static_cast<uint8_t>(t);
            *to_nxt++ = static_cast<uint8_t>(t >> 8);
            t = static_cast<uint16_t>(0xDC00 | (wc & 0x03FF));
            *to_nxt++ = static_cast<uint8_t>(t);
            *to_nxt++ = static_cast<uint8_t>(t >> 8);
        }
    }
    return codecvt_base::ok;
}

static
codecvt_base::result
utf16le_to_ucs4(const uint8_t* frm, const uint8_t* frm_end, const uint8_t*& frm_nxt,
                uint32_t* to, uint32_t* to_end, uint32_t*& to_nxt,
                unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 2 && frm_nxt[0] == 0xFF && frm_nxt[1] == 0xFE)
            frm_nxt += 2;
    }
    for (; frm_nxt < frm_end - 1 && to_nxt < to_end; ++to_nxt)
    {
        uint16_t c1 = frm_nxt[1] << 8 | frm_nxt[0];
        if ((c1 & 0xFC00) == 0xDC00)
            return codecvt_base::error;
        if ((c1 & 0xFC00) != 0xD800)
        {
            if (c1 > Maxcode)
                return codecvt_base::error;
            *to_nxt = static_cast<uint32_t>(c1);
            frm_nxt += 2;
        }
        else
        {
            if (frm_end-frm_nxt < 4)
                return codecvt_base::partial;
            uint16_t c2 = frm_nxt[3] << 8 | frm_nxt[2];
            if ((c2 & 0xFC00) != 0xDC00)
                return codecvt_base::error;
            uint32_t t = static_cast<uint32_t>(
                    ((((c1 & 0x03C0) >> 6) + 1) << 16)
                  |   ((c1 & 0x003F) << 10)
                  |    (c2 & 0x03FF));
            if (t > Maxcode)
                return codecvt_base::error;
            *to_nxt = t;
            frm_nxt += 4;
        }
    }
    return frm_nxt < frm_end ? codecvt_base::partial : codecvt_base::ok;
}

static
int
utf16le_to_ucs4_length(const uint8_t* frm, const uint8_t* frm_end,
                       size_t mx, unsigned long Maxcode = 0x10FFFF,
                       codecvt_mode mode = codecvt_mode(0))
{
    const uint8_t* frm_nxt = frm;
    frm_nxt = frm;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 2 && frm_nxt[0] == 0xFF && frm_nxt[1] == 0xFE)
            frm_nxt += 2;
    }
    for (size_t nchar32_t = 0; frm_nxt < frm_end - 1 && nchar32_t < mx; ++nchar32_t)
    {
        uint16_t c1 = frm_nxt[1] << 8 | frm_nxt[0];
        if ((c1 & 0xFC00) == 0xDC00)
            break;
        if ((c1 & 0xFC00) != 0xD800)
        {
            if (c1 > Maxcode)
                break;
            frm_nxt += 2;
        }
        else
        {
            if (frm_end-frm_nxt < 4)
                break;
            uint16_t c2 = frm_nxt[3] << 8 | frm_nxt[2];
            if ((c2 & 0xFC00) != 0xDC00)
                break;
            uint32_t t = static_cast<uint32_t>(
                    ((((c1 & 0x03C0) >> 6) + 1) << 16)
                  |   ((c1 & 0x003F) << 10)
                  |    (c2 & 0x03FF));
            if (t > Maxcode)
                break;
            frm_nxt += 4;
        }
    }
    return static_cast<int>(frm_nxt - frm);
}

static
codecvt_base::result
ucs2_to_utf16be(const uint16_t* frm, const uint16_t* frm_end, const uint16_t*& frm_nxt,
                uint8_t* to, uint8_t* to_end, uint8_t*& to_nxt,
                unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & generate_header)
    {
        if (to_end-to_nxt < 2)
            return codecvt_base::partial;
        *to_nxt++ = static_cast<uint8_t>(0xFE);
        *to_nxt++ = static_cast<uint8_t>(0xFF);
    }
    for (; frm_nxt < frm_end; ++frm_nxt)
    {
        uint16_t wc = *frm_nxt;
        if ((wc & 0xF800) == 0xD800 || wc > Maxcode)
            return codecvt_base::error;
        if (to_end-to_nxt < 2)
            return codecvt_base::partial;
        *to_nxt++ = static_cast<uint8_t>(wc >> 8);
        *to_nxt++ = static_cast<uint8_t>(wc);
    }
    return codecvt_base::ok;
}

static
codecvt_base::result
utf16be_to_ucs2(const uint8_t* frm, const uint8_t* frm_end, const uint8_t*& frm_nxt,
                uint16_t* to, uint16_t* to_end, uint16_t*& to_nxt,
                unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 2 && frm_nxt[0] == 0xFE && frm_nxt[1] == 0xFF)
            frm_nxt += 2;
    }
    for (; frm_nxt < frm_end - 1 && to_nxt < to_end; ++to_nxt)
    {
        uint16_t c1 = frm_nxt[0] << 8 | frm_nxt[1];
        if ((c1 & 0xF800) == 0xD800 || c1 > Maxcode)
            return codecvt_base::error;
        *to_nxt = c1;
        frm_nxt += 2;
    }
    return frm_nxt < frm_end ? codecvt_base::partial : codecvt_base::ok;
}

static
int
utf16be_to_ucs2_length(const uint8_t* frm, const uint8_t* frm_end,
                       size_t mx, unsigned long Maxcode = 0x10FFFF,
                       codecvt_mode mode = codecvt_mode(0))
{
    const uint8_t* frm_nxt = frm;
    frm_nxt = frm;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 2 && frm_nxt[0] == 0xFE && frm_nxt[1] == 0xFF)
            frm_nxt += 2;
    }
    for (size_t nchar16_t = 0; frm_nxt < frm_end - 1 && nchar16_t < mx; ++nchar16_t)
    {
        uint16_t c1 = frm_nxt[0] << 8 | frm_nxt[1];
        if ((c1 & 0xF800) == 0xD800 || c1 > Maxcode)
            break;
        frm_nxt += 2;
    }
    return static_cast<int>(frm_nxt - frm);
}

static
codecvt_base::result
ucs2_to_utf16le(const uint16_t* frm, const uint16_t* frm_end, const uint16_t*& frm_nxt,
                uint8_t* to, uint8_t* to_end, uint8_t*& to_nxt,
                unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & generate_header)
    {
        if (to_end-to_nxt < 2)
            return codecvt_base::partial;
        *to_nxt++ = static_cast<uint8_t>(0xFF);
        *to_nxt++ = static_cast<uint8_t>(0xFE);
    }
    for (; frm_nxt < frm_end; ++frm_nxt)
    {
        uint16_t wc = *frm_nxt;
        if ((wc & 0xF800) == 0xD800 || wc > Maxcode)
            return codecvt_base::error;
        if (to_end-to_nxt < 2)
            return codecvt_base::partial;
        *to_nxt++ = static_cast<uint8_t>(wc);
        *to_nxt++ = static_cast<uint8_t>(wc >> 8);
    }
    return codecvt_base::ok;
}

static
codecvt_base::result
utf16le_to_ucs2(const uint8_t* frm, const uint8_t* frm_end, const uint8_t*& frm_nxt,
                uint16_t* to, uint16_t* to_end, uint16_t*& to_nxt,
                unsigned long Maxcode = 0x10FFFF, codecvt_mode mode = codecvt_mode(0))
{
    frm_nxt = frm;
    to_nxt = to;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 2 && frm_nxt[0] == 0xFF && frm_nxt[1] == 0xFE)
            frm_nxt += 2;
    }
    for (; frm_nxt < frm_end - 1 && to_nxt < to_end; ++to_nxt)
    {
        uint16_t c1 = frm_nxt[1] << 8 | frm_nxt[0];
        if ((c1 & 0xF800) == 0xD800 || c1 > Maxcode)
            return codecvt_base::error;
        *to_nxt = c1;
        frm_nxt += 2;
    }
    return frm_nxt < frm_end ? codecvt_base::partial : codecvt_base::ok;
}

static
int
utf16le_to_ucs2_length(const uint8_t* frm, const uint8_t* frm_end,
                       size_t mx, unsigned long Maxcode = 0x10FFFF,
                       codecvt_mode mode = codecvt_mode(0))
{
    const uint8_t* frm_nxt = frm;
    frm_nxt = frm;
    if (mode & consume_header)
    {
        if (frm_end-frm_nxt >= 2 && frm_nxt[0] == 0xFF && frm_nxt[1] == 0xFE)
            frm_nxt += 2;
    }
    for (size_t nchar16_t = 0; frm_nxt < frm_end - 1 && nchar16_t < mx; ++nchar16_t)
    {
        uint16_t c1 = frm_nxt[1] << 8 | frm_nxt[0];
        if ((c1 & 0xF800) == 0xD800 || c1 > Maxcode)
            break;
        frm_nxt += 2;
    }
    return static_cast<int>(frm_nxt - frm);
}

// template <> class codecvt<char16_t, char, mbstate_t>

locale::id codecvt<char16_t, char, mbstate_t>::id;

codecvt<char16_t, char, mbstate_t>::~codecvt()
{
}

codecvt<char16_t, char, mbstate_t>::result
codecvt<char16_t, char, mbstate_t>::do_out(state_type&,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    const uint16_t* _frm = reinterpret_cast<const uint16_t*>(frm);
    const uint16_t* _frm_end = reinterpret_cast<const uint16_t*>(frm_end);
    const uint16_t* _frm_nxt = _frm;
    uint8_t* _to = reinterpret_cast<uint8_t*>(to);
    uint8_t* _to_end = reinterpret_cast<uint8_t*>(to_end);
    uint8_t* _to_nxt = _to;
    result r = utf16_to_utf8(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

codecvt<char16_t, char, mbstate_t>::result
codecvt<char16_t, char, mbstate_t>::do_in(state_type&,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    const uint8_t* _frm_nxt = _frm;
    uint16_t* _to = reinterpret_cast<uint16_t*>(to);
    uint16_t* _to_end = reinterpret_cast<uint16_t*>(to_end);
    uint16_t* _to_nxt = _to;
    result r = utf8_to_utf16(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

codecvt<char16_t, char, mbstate_t>::result
codecvt<char16_t, char, mbstate_t>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
codecvt<char16_t, char, mbstate_t>::do_encoding() const  _NOEXCEPT
{
    return 0;
}

bool
codecvt<char16_t, char, mbstate_t>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
codecvt<char16_t, char, mbstate_t>::do_length(state_type&,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    return utf8_to_utf16_length(_frm, _frm_end, mx);
}

int
codecvt<char16_t, char, mbstate_t>::do_max_length() const  _NOEXCEPT
{
    return 4;
}

// template <> class codecvt<char32_t, char, mbstate_t>

locale::id codecvt<char32_t, char, mbstate_t>::id;

codecvt<char32_t, char, mbstate_t>::~codecvt()
{
}

codecvt<char32_t, char, mbstate_t>::result
codecvt<char32_t, char, mbstate_t>::do_out(state_type&,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    const uint32_t* _frm = reinterpret_cast<const uint32_t*>(frm);
    const uint32_t* _frm_end = reinterpret_cast<const uint32_t*>(frm_end);
    const uint32_t* _frm_nxt = _frm;
    uint8_t* _to = reinterpret_cast<uint8_t*>(to);
    uint8_t* _to_end = reinterpret_cast<uint8_t*>(to_end);
    uint8_t* _to_nxt = _to;
    result r = ucs4_to_utf8(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

codecvt<char32_t, char, mbstate_t>::result
codecvt<char32_t, char, mbstate_t>::do_in(state_type&,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    const uint8_t* _frm_nxt = _frm;
    uint32_t* _to = reinterpret_cast<uint32_t*>(to);
    uint32_t* _to_end = reinterpret_cast<uint32_t*>(to_end);
    uint32_t* _to_nxt = _to;
    result r = utf8_to_ucs4(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

codecvt<char32_t, char, mbstate_t>::result
codecvt<char32_t, char, mbstate_t>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
codecvt<char32_t, char, mbstate_t>::do_encoding() const  _NOEXCEPT
{
    return 0;
}

bool
codecvt<char32_t, char, mbstate_t>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
codecvt<char32_t, char, mbstate_t>::do_length(state_type&,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    return utf8_to_ucs4_length(_frm, _frm_end, mx);
}

int
codecvt<char32_t, char, mbstate_t>::do_max_length() const  _NOEXCEPT
{
    return 4;
}

// __codecvt_utf8<wchar_t>

__codecvt_utf8<wchar_t>::result
__codecvt_utf8<wchar_t>::do_out(state_type&,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    const uint32_t* _frm = reinterpret_cast<const uint32_t*>(frm);
    const uint32_t* _frm_end = reinterpret_cast<const uint32_t*>(frm_end);
    const uint32_t* _frm_nxt = _frm;
    uint8_t* _to = reinterpret_cast<uint8_t*>(to);
    uint8_t* _to_end = reinterpret_cast<uint8_t*>(to_end);
    uint8_t* _to_nxt = _to;
    result r = ucs4_to_utf8(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                            _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf8<wchar_t>::result
__codecvt_utf8<wchar_t>::do_in(state_type&,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    const uint8_t* _frm_nxt = _frm;
    uint32_t* _to = reinterpret_cast<uint32_t*>(to);
    uint32_t* _to_end = reinterpret_cast<uint32_t*>(to_end);
    uint32_t* _to_nxt = _to;
    result r = utf8_to_ucs4(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                            _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf8<wchar_t>::result
__codecvt_utf8<wchar_t>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
__codecvt_utf8<wchar_t>::do_encoding() const  _NOEXCEPT
{
    return 0;
}

bool
__codecvt_utf8<wchar_t>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
__codecvt_utf8<wchar_t>::do_length(state_type&,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    return utf8_to_ucs4_length(_frm, _frm_end, mx, _Maxcode_, _Mode_);
}

int
__codecvt_utf8<wchar_t>::do_max_length() const  _NOEXCEPT
{
    if (_Mode_ & consume_header)
        return 7;
    return 4;
}

// __codecvt_utf8<char16_t>

__codecvt_utf8<char16_t>::result
__codecvt_utf8<char16_t>::do_out(state_type&,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    const uint16_t* _frm = reinterpret_cast<const uint16_t*>(frm);
    const uint16_t* _frm_end = reinterpret_cast<const uint16_t*>(frm_end);
    const uint16_t* _frm_nxt = _frm;
    uint8_t* _to = reinterpret_cast<uint8_t*>(to);
    uint8_t* _to_end = reinterpret_cast<uint8_t*>(to_end);
    uint8_t* _to_nxt = _to;
    result r = ucs2_to_utf8(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                            _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf8<char16_t>::result
__codecvt_utf8<char16_t>::do_in(state_type&,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    const uint8_t* _frm_nxt = _frm;
    uint16_t* _to = reinterpret_cast<uint16_t*>(to);
    uint16_t* _to_end = reinterpret_cast<uint16_t*>(to_end);
    uint16_t* _to_nxt = _to;
    result r = utf8_to_ucs2(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                            _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf8<char16_t>::result
__codecvt_utf8<char16_t>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
__codecvt_utf8<char16_t>::do_encoding() const  _NOEXCEPT
{
    return 0;
}

bool
__codecvt_utf8<char16_t>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
__codecvt_utf8<char16_t>::do_length(state_type&,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    return utf8_to_ucs2_length(_frm, _frm_end, mx, _Maxcode_, _Mode_);
}

int
__codecvt_utf8<char16_t>::do_max_length() const  _NOEXCEPT
{
    if (_Mode_ & consume_header)
        return 6;
    return 3;
}

// __codecvt_utf8<char32_t>

__codecvt_utf8<char32_t>::result
__codecvt_utf8<char32_t>::do_out(state_type&,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    const uint32_t* _frm = reinterpret_cast<const uint32_t*>(frm);
    const uint32_t* _frm_end = reinterpret_cast<const uint32_t*>(frm_end);
    const uint32_t* _frm_nxt = _frm;
    uint8_t* _to = reinterpret_cast<uint8_t*>(to);
    uint8_t* _to_end = reinterpret_cast<uint8_t*>(to_end);
    uint8_t* _to_nxt = _to;
    result r = ucs4_to_utf8(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                            _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf8<char32_t>::result
__codecvt_utf8<char32_t>::do_in(state_type&,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    const uint8_t* _frm_nxt = _frm;
    uint32_t* _to = reinterpret_cast<uint32_t*>(to);
    uint32_t* _to_end = reinterpret_cast<uint32_t*>(to_end);
    uint32_t* _to_nxt = _to;
    result r = utf8_to_ucs4(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                            _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf8<char32_t>::result
__codecvt_utf8<char32_t>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
__codecvt_utf8<char32_t>::do_encoding() const  _NOEXCEPT
{
    return 0;
}

bool
__codecvt_utf8<char32_t>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
__codecvt_utf8<char32_t>::do_length(state_type&,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    return utf8_to_ucs4_length(_frm, _frm_end, mx, _Maxcode_, _Mode_);
}

int
__codecvt_utf8<char32_t>::do_max_length() const  _NOEXCEPT
{
    if (_Mode_ & consume_header)
        return 7;
    return 4;
}

// __codecvt_utf16<wchar_t, false>

__codecvt_utf16<wchar_t, false>::result
__codecvt_utf16<wchar_t, false>::do_out(state_type&,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    const uint32_t* _frm = reinterpret_cast<const uint32_t*>(frm);
    const uint32_t* _frm_end = reinterpret_cast<const uint32_t*>(frm_end);
    const uint32_t* _frm_nxt = _frm;
    uint8_t* _to = reinterpret_cast<uint8_t*>(to);
    uint8_t* _to_end = reinterpret_cast<uint8_t*>(to_end);
    uint8_t* _to_nxt = _to;
    result r = ucs4_to_utf16be(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                               _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf16<wchar_t, false>::result
__codecvt_utf16<wchar_t, false>::do_in(state_type&,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    const uint8_t* _frm_nxt = _frm;
    uint32_t* _to = reinterpret_cast<uint32_t*>(to);
    uint32_t* _to_end = reinterpret_cast<uint32_t*>(to_end);
    uint32_t* _to_nxt = _to;
    result r = utf16be_to_ucs4(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                               _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf16<wchar_t, false>::result
__codecvt_utf16<wchar_t, false>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
__codecvt_utf16<wchar_t, false>::do_encoding() const  _NOEXCEPT
{
    return 0;
}

bool
__codecvt_utf16<wchar_t, false>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
__codecvt_utf16<wchar_t, false>::do_length(state_type&,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    return utf16be_to_ucs4_length(_frm, _frm_end, mx, _Maxcode_, _Mode_);
}

int
__codecvt_utf16<wchar_t, false>::do_max_length() const  _NOEXCEPT
{
    if (_Mode_ & consume_header)
        return 6;
    return 4;
}

// __codecvt_utf16<wchar_t, true>

__codecvt_utf16<wchar_t, true>::result
__codecvt_utf16<wchar_t, true>::do_out(state_type&,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    const uint32_t* _frm = reinterpret_cast<const uint32_t*>(frm);
    const uint32_t* _frm_end = reinterpret_cast<const uint32_t*>(frm_end);
    const uint32_t* _frm_nxt = _frm;
    uint8_t* _to = reinterpret_cast<uint8_t*>(to);
    uint8_t* _to_end = reinterpret_cast<uint8_t*>(to_end);
    uint8_t* _to_nxt = _to;
    result r = ucs4_to_utf16le(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                               _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf16<wchar_t, true>::result
__codecvt_utf16<wchar_t, true>::do_in(state_type&,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    const uint8_t* _frm_nxt = _frm;
    uint32_t* _to = reinterpret_cast<uint32_t*>(to);
    uint32_t* _to_end = reinterpret_cast<uint32_t*>(to_end);
    uint32_t* _to_nxt = _to;
    result r = utf16le_to_ucs4(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                               _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf16<wchar_t, true>::result
__codecvt_utf16<wchar_t, true>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
__codecvt_utf16<wchar_t, true>::do_encoding() const  _NOEXCEPT
{
    return 0;
}

bool
__codecvt_utf16<wchar_t, true>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
__codecvt_utf16<wchar_t, true>::do_length(state_type&,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    return utf16le_to_ucs4_length(_frm, _frm_end, mx, _Maxcode_, _Mode_);
}

int
__codecvt_utf16<wchar_t, true>::do_max_length() const  _NOEXCEPT
{
    if (_Mode_ & consume_header)
        return 6;
    return 4;
}

// __codecvt_utf16<char16_t, false>

__codecvt_utf16<char16_t, false>::result
__codecvt_utf16<char16_t, false>::do_out(state_type&,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    const uint16_t* _frm = reinterpret_cast<const uint16_t*>(frm);
    const uint16_t* _frm_end = reinterpret_cast<const uint16_t*>(frm_end);
    const uint16_t* _frm_nxt = _frm;
    uint8_t* _to = reinterpret_cast<uint8_t*>(to);
    uint8_t* _to_end = reinterpret_cast<uint8_t*>(to_end);
    uint8_t* _to_nxt = _to;
    result r = ucs2_to_utf16be(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                               _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf16<char16_t, false>::result
__codecvt_utf16<char16_t, false>::do_in(state_type&,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    const uint8_t* _frm_nxt = _frm;
    uint16_t* _to = reinterpret_cast<uint16_t*>(to);
    uint16_t* _to_end = reinterpret_cast<uint16_t*>(to_end);
    uint16_t* _to_nxt = _to;
    result r = utf16be_to_ucs2(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                               _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf16<char16_t, false>::result
__codecvt_utf16<char16_t, false>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
__codecvt_utf16<char16_t, false>::do_encoding() const  _NOEXCEPT
{
    return 0;
}

bool
__codecvt_utf16<char16_t, false>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
__codecvt_utf16<char16_t, false>::do_length(state_type&,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    return utf16be_to_ucs2_length(_frm, _frm_end, mx, _Maxcode_, _Mode_);
}

int
__codecvt_utf16<char16_t, false>::do_max_length() const  _NOEXCEPT
{
    if (_Mode_ & consume_header)
        return 4;
    return 2;
}

// __codecvt_utf16<char16_t, true>

__codecvt_utf16<char16_t, true>::result
__codecvt_utf16<char16_t, true>::do_out(state_type&,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    const uint16_t* _frm = reinterpret_cast<const uint16_t*>(frm);
    const uint16_t* _frm_end = reinterpret_cast<const uint16_t*>(frm_end);
    const uint16_t* _frm_nxt = _frm;
    uint8_t* _to = reinterpret_cast<uint8_t*>(to);
    uint8_t* _to_end = reinterpret_cast<uint8_t*>(to_end);
    uint8_t* _to_nxt = _to;
    result r = ucs2_to_utf16le(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                               _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf16<char16_t, true>::result
__codecvt_utf16<char16_t, true>::do_in(state_type&,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    const uint8_t* _frm_nxt = _frm;
    uint16_t* _to = reinterpret_cast<uint16_t*>(to);
    uint16_t* _to_end = reinterpret_cast<uint16_t*>(to_end);
    uint16_t* _to_nxt = _to;
    result r = utf16le_to_ucs2(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                               _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf16<char16_t, true>::result
__codecvt_utf16<char16_t, true>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
__codecvt_utf16<char16_t, true>::do_encoding() const  _NOEXCEPT
{
    return 0;
}

bool
__codecvt_utf16<char16_t, true>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
__codecvt_utf16<char16_t, true>::do_length(state_type&,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    return utf16le_to_ucs2_length(_frm, _frm_end, mx, _Maxcode_, _Mode_);
}

int
__codecvt_utf16<char16_t, true>::do_max_length() const  _NOEXCEPT
{
    if (_Mode_ & consume_header)
        return 4;
    return 2;
}

// __codecvt_utf16<char32_t, false>

__codecvt_utf16<char32_t, false>::result
__codecvt_utf16<char32_t, false>::do_out(state_type&,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    const uint32_t* _frm = reinterpret_cast<const uint32_t*>(frm);
    const uint32_t* _frm_end = reinterpret_cast<const uint32_t*>(frm_end);
    const uint32_t* _frm_nxt = _frm;
    uint8_t* _to = reinterpret_cast<uint8_t*>(to);
    uint8_t* _to_end = reinterpret_cast<uint8_t*>(to_end);
    uint8_t* _to_nxt = _to;
    result r = ucs4_to_utf16be(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                               _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf16<char32_t, false>::result
__codecvt_utf16<char32_t, false>::do_in(state_type&,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    const uint8_t* _frm_nxt = _frm;
    uint32_t* _to = reinterpret_cast<uint32_t*>(to);
    uint32_t* _to_end = reinterpret_cast<uint32_t*>(to_end);
    uint32_t* _to_nxt = _to;
    result r = utf16be_to_ucs4(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                               _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf16<char32_t, false>::result
__codecvt_utf16<char32_t, false>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
__codecvt_utf16<char32_t, false>::do_encoding() const  _NOEXCEPT
{
    return 0;
}

bool
__codecvt_utf16<char32_t, false>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
__codecvt_utf16<char32_t, false>::do_length(state_type&,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    return utf16be_to_ucs4_length(_frm, _frm_end, mx, _Maxcode_, _Mode_);
}

int
__codecvt_utf16<char32_t, false>::do_max_length() const  _NOEXCEPT
{
    if (_Mode_ & consume_header)
        return 6;
    return 4;
}

// __codecvt_utf16<char32_t, true>

__codecvt_utf16<char32_t, true>::result
__codecvt_utf16<char32_t, true>::do_out(state_type&,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    const uint32_t* _frm = reinterpret_cast<const uint32_t*>(frm);
    const uint32_t* _frm_end = reinterpret_cast<const uint32_t*>(frm_end);
    const uint32_t* _frm_nxt = _frm;
    uint8_t* _to = reinterpret_cast<uint8_t*>(to);
    uint8_t* _to_end = reinterpret_cast<uint8_t*>(to_end);
    uint8_t* _to_nxt = _to;
    result r = ucs4_to_utf16le(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                               _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf16<char32_t, true>::result
__codecvt_utf16<char32_t, true>::do_in(state_type&,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    const uint8_t* _frm_nxt = _frm;
    uint32_t* _to = reinterpret_cast<uint32_t*>(to);
    uint32_t* _to_end = reinterpret_cast<uint32_t*>(to_end);
    uint32_t* _to_nxt = _to;
    result r = utf16le_to_ucs4(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                               _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf16<char32_t, true>::result
__codecvt_utf16<char32_t, true>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
__codecvt_utf16<char32_t, true>::do_encoding() const  _NOEXCEPT
{
    return 0;
}

bool
__codecvt_utf16<char32_t, true>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
__codecvt_utf16<char32_t, true>::do_length(state_type&,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    return utf16le_to_ucs4_length(_frm, _frm_end, mx, _Maxcode_, _Mode_);
}

int
__codecvt_utf16<char32_t, true>::do_max_length() const  _NOEXCEPT
{
    if (_Mode_ & consume_header)
        return 6;
    return 4;
}

// __codecvt_utf8_utf16<wchar_t>

__codecvt_utf8_utf16<wchar_t>::result
__codecvt_utf8_utf16<wchar_t>::do_out(state_type&,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    const uint32_t* _frm = reinterpret_cast<const uint32_t*>(frm);
    const uint32_t* _frm_end = reinterpret_cast<const uint32_t*>(frm_end);
    const uint32_t* _frm_nxt = _frm;
    uint8_t* _to = reinterpret_cast<uint8_t*>(to);
    uint8_t* _to_end = reinterpret_cast<uint8_t*>(to_end);
    uint8_t* _to_nxt = _to;
    result r = utf16_to_utf8(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                             _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf8_utf16<wchar_t>::result
__codecvt_utf8_utf16<wchar_t>::do_in(state_type&,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    const uint8_t* _frm_nxt = _frm;
    uint32_t* _to = reinterpret_cast<uint32_t*>(to);
    uint32_t* _to_end = reinterpret_cast<uint32_t*>(to_end);
    uint32_t* _to_nxt = _to;
    result r = utf8_to_utf16(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                             _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf8_utf16<wchar_t>::result
__codecvt_utf8_utf16<wchar_t>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
__codecvt_utf8_utf16<wchar_t>::do_encoding() const  _NOEXCEPT
{
    return 0;
}

bool
__codecvt_utf8_utf16<wchar_t>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
__codecvt_utf8_utf16<wchar_t>::do_length(state_type&,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    return utf8_to_utf16_length(_frm, _frm_end, mx, _Maxcode_, _Mode_);
}

int
__codecvt_utf8_utf16<wchar_t>::do_max_length() const  _NOEXCEPT
{
    if (_Mode_ & consume_header)
        return 7;
    return 4;
}

// __codecvt_utf8_utf16<char16_t>

__codecvt_utf8_utf16<char16_t>::result
__codecvt_utf8_utf16<char16_t>::do_out(state_type&,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    const uint16_t* _frm = reinterpret_cast<const uint16_t*>(frm);
    const uint16_t* _frm_end = reinterpret_cast<const uint16_t*>(frm_end);
    const uint16_t* _frm_nxt = _frm;
    uint8_t* _to = reinterpret_cast<uint8_t*>(to);
    uint8_t* _to_end = reinterpret_cast<uint8_t*>(to_end);
    uint8_t* _to_nxt = _to;
    result r = utf16_to_utf8(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                             _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf8_utf16<char16_t>::result
__codecvt_utf8_utf16<char16_t>::do_in(state_type&,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    const uint8_t* _frm_nxt = _frm;
    uint16_t* _to = reinterpret_cast<uint16_t*>(to);
    uint16_t* _to_end = reinterpret_cast<uint16_t*>(to_end);
    uint16_t* _to_nxt = _to;
    result r = utf8_to_utf16(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                             _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf8_utf16<char16_t>::result
__codecvt_utf8_utf16<char16_t>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
__codecvt_utf8_utf16<char16_t>::do_encoding() const  _NOEXCEPT
{
    return 0;
}

bool
__codecvt_utf8_utf16<char16_t>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
__codecvt_utf8_utf16<char16_t>::do_length(state_type&,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    return utf8_to_utf16_length(_frm, _frm_end, mx, _Maxcode_, _Mode_);
}

int
__codecvt_utf8_utf16<char16_t>::do_max_length() const  _NOEXCEPT
{
    if (_Mode_ & consume_header)
        return 7;
    return 4;
}

// __codecvt_utf8_utf16<char32_t>

__codecvt_utf8_utf16<char32_t>::result
__codecvt_utf8_utf16<char32_t>::do_out(state_type&,
    const intern_type* frm, const intern_type* frm_end, const intern_type*& frm_nxt,
    extern_type* to, extern_type* to_end, extern_type*& to_nxt) const
{
    const uint32_t* _frm = reinterpret_cast<const uint32_t*>(frm);
    const uint32_t* _frm_end = reinterpret_cast<const uint32_t*>(frm_end);
    const uint32_t* _frm_nxt = _frm;
    uint8_t* _to = reinterpret_cast<uint8_t*>(to);
    uint8_t* _to_end = reinterpret_cast<uint8_t*>(to_end);
    uint8_t* _to_nxt = _to;
    result r = utf16_to_utf8(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                             _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf8_utf16<char32_t>::result
__codecvt_utf8_utf16<char32_t>::do_in(state_type&,
    const extern_type* frm, const extern_type* frm_end, const extern_type*& frm_nxt,
    intern_type* to, intern_type* to_end, intern_type*& to_nxt) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    const uint8_t* _frm_nxt = _frm;
    uint32_t* _to = reinterpret_cast<uint32_t*>(to);
    uint32_t* _to_end = reinterpret_cast<uint32_t*>(to_end);
    uint32_t* _to_nxt = _to;
    result r = utf8_to_utf16(_frm, _frm_end, _frm_nxt, _to, _to_end, _to_nxt,
                             _Maxcode_, _Mode_);
    frm_nxt = frm + (_frm_nxt - _frm);
    to_nxt = to + (_to_nxt - _to);
    return r;
}

__codecvt_utf8_utf16<char32_t>::result
__codecvt_utf8_utf16<char32_t>::do_unshift(state_type&,
    extern_type* to, extern_type*, extern_type*& to_nxt) const
{
    to_nxt = to;
    return noconv;
}

int
__codecvt_utf8_utf16<char32_t>::do_encoding() const  _NOEXCEPT
{
    return 0;
}

bool
__codecvt_utf8_utf16<char32_t>::do_always_noconv() const  _NOEXCEPT
{
    return false;
}

int
__codecvt_utf8_utf16<char32_t>::do_length(state_type&,
    const extern_type* frm, const extern_type* frm_end, size_t mx) const
{
    const uint8_t* _frm = reinterpret_cast<const uint8_t*>(frm);
    const uint8_t* _frm_end = reinterpret_cast<const uint8_t*>(frm_end);
    return utf8_to_utf16_length(_frm, _frm_end, mx, _Maxcode_, _Mode_);
}

int
__codecvt_utf8_utf16<char32_t>::do_max_length() const  _NOEXCEPT
{
    if (_Mode_ & consume_header)
        return 7;
    return 4;
}

// __narrow_to_utf8<16>

__narrow_to_utf8<16>::~__narrow_to_utf8()
{
}

// __narrow_to_utf8<32>

__narrow_to_utf8<32>::~__narrow_to_utf8()
{
}

// __widen_from_utf8<16>

__widen_from_utf8<16>::~__widen_from_utf8()
{
}

// __widen_from_utf8<32>

__widen_from_utf8<32>::~__widen_from_utf8()
{
}

// numpunct<char> && numpunct<wchar_t>

locale::id numpunct< char  >::id;
locale::id numpunct<wchar_t>::id;

numpunct<char>::numpunct(size_t refs)
    : locale::facet(refs),
      __decimal_point_('.'),
      __thousands_sep_(',')
{
}

numpunct<wchar_t>::numpunct(size_t refs)
    : locale::facet(refs),
      __decimal_point_(L'.'),
      __thousands_sep_(L',')
{
}

numpunct<char>::~numpunct()
{
}

numpunct<wchar_t>::~numpunct()
{
}

 char   numpunct< char  >::do_decimal_point() const {return __decimal_point_;}
wchar_t numpunct<wchar_t>::do_decimal_point() const {return __decimal_point_;}

 char   numpunct< char  >::do_thousands_sep() const {return __thousands_sep_;}
wchar_t numpunct<wchar_t>::do_thousands_sep() const {return __thousands_sep_;}

string numpunct< char  >::do_grouping() const {return __grouping_;}
string numpunct<wchar_t>::do_grouping() const {return __grouping_;}

 string numpunct< char  >::do_truename() const {return "true";}
wstring numpunct<wchar_t>::do_truename() const {return L"true";}

 string numpunct< char  >::do_falsename() const {return "false";}
wstring numpunct<wchar_t>::do_falsename() const {return L"false";}

// numpunct_byname<char>

numpunct_byname<char>::numpunct_byname(const char* nm, size_t refs)
    : numpunct<char>(refs)
{
    __init(nm);
}

numpunct_byname<char>::numpunct_byname(const string& nm, size_t refs)
    : numpunct<char>(refs)
{
    __init(nm.c_str());
}

numpunct_byname<char>::~numpunct_byname()
{
}

void
numpunct_byname<char>::__init(const char* nm)
{
    if (strcmp(nm, "C") != 0)
    {
        __locale_unique_ptr loc(newlocale(LC_ALL_MASK, nm, 0), freelocale);
#ifndef _LIBCPP_NO_EXCEPTIONS
        if (loc == 0)
            throw runtime_error("numpunct_byname<char>::numpunct_byname"
                                " failed to construct for " + string(nm));
#endif  // _LIBCPP_NO_EXCEPTIONS
#ifdef _LIBCPP_STABLE_APPLE_ABI
        lconv* lc = localeconv_l(loc.get());
#else
        lconv* lc = __localeconv_l(loc.get());
#endif
        if (*lc->decimal_point)
            __decimal_point_ = *lc->decimal_point;
        if (*lc->thousands_sep)
            __thousands_sep_ = *lc->thousands_sep;
        __grouping_ = lc->grouping;
        // localization for truename and falsename is not available
    }
}

// numpunct_byname<wchar_t>

numpunct_byname<wchar_t>::numpunct_byname(const char* nm, size_t refs)
    : numpunct<wchar_t>(refs)
{
    __init(nm);
}

numpunct_byname<wchar_t>::numpunct_byname(const string& nm, size_t refs)
    : numpunct<wchar_t>(refs)
{
    __init(nm.c_str());
}

numpunct_byname<wchar_t>::~numpunct_byname()
{
}

void
numpunct_byname<wchar_t>::__init(const char* nm)
{
    if (strcmp(nm, "C") != 0)
    {
        __locale_unique_ptr loc(newlocale(LC_ALL_MASK, nm, 0), freelocale);
#ifndef _LIBCPP_NO_EXCEPTIONS
        if (loc == 0)
            throw runtime_error("numpunct_byname<char>::numpunct_byname"
                                " failed to construct for " + string(nm));
#endif  // _LIBCPP_NO_EXCEPTIONS
#ifdef _LIBCPP_STABLE_APPLE_ABI
        lconv* lc = localeconv_l(loc.get());
#else
        lconv* lc = __localeconv_l(loc.get());
#endif
        if (*lc->decimal_point)
            __decimal_point_ = *lc->decimal_point;
        if (*lc->thousands_sep)
            __thousands_sep_ = *lc->thousands_sep;
        __grouping_ = lc->grouping;
        // locallization for truename and falsename is not available
    }
}

// num_get helpers

int
__num_get_base::__get_base(ios_base& iob)
{
    ios_base::fmtflags __basefield = iob.flags() & ios_base::basefield;
    if (__basefield == ios_base::oct)
        return 8;
    else if (__basefield == ios_base::hex)
        return 16;
    else if (__basefield == 0)
        return 0;
    return 10;
}

const char __num_get_base::__src[33] = "0123456789abcdefABCDEFxX+-pPiInN";

void
__check_grouping(const string& __grouping, unsigned* __g, unsigned* __g_end,
                 ios_base::iostate& __err)
{
    if (__grouping.size() != 0)
    {
        reverse(__g, __g_end);
        const char* __ig = __grouping.data();
        const char* __eg = __ig + __grouping.size();
        for (unsigned* __r = __g; __r < __g_end-1; ++__r)
        {
            if (0 < *__ig && *__ig < numeric_limits<char>::max())
            {
                if (*__ig != *__r)
                {
                    __err = ios_base::failbit;
                    return;
                }
            }
            if (__eg - __ig > 1)
                ++__ig;
        }
        if (0 < *__ig && *__ig < numeric_limits<char>::max())
        {
            if (*__ig < __g_end[-1] || __g_end[-1] == 0)
                __err = ios_base::failbit;
        }
    }
}

void
__num_put_base::__format_int(char* __fmtp, const char* __len, bool __signd,
                             ios_base::fmtflags __flags)
{
    if (__flags & ios_base::showpos)
        *__fmtp++ = '+';
    if (__flags & ios_base::showbase)
        *__fmtp++ = '#';
    while(*__len)
        *__fmtp++ = *__len++;
    if ((__flags & ios_base::basefield) == ios_base::oct)
        *__fmtp = 'o';
    else if ((__flags & ios_base::basefield) == ios_base::hex)
    {
        if (__flags & ios_base::uppercase)
            *__fmtp = 'X';
        else
            *__fmtp = 'x';
    }
    else if (__signd)
        *__fmtp = 'd';
    else
        *__fmtp = 'u';
}

bool
__num_put_base::__format_float(char* __fmtp, const char* __len,
                               ios_base::fmtflags __flags)
{
    bool specify_precision = true;
    if (__flags & ios_base::showpos)
        *__fmtp++ = '+';
    if (__flags & ios_base::showpoint)
        *__fmtp++ = '#';
    ios_base::fmtflags floatfield = __flags & ios_base::floatfield;
    bool uppercase = __flags & ios_base::uppercase;
    if (floatfield == (ios_base::fixed | ios_base::scientific))
        specify_precision = false;
    else
    {
        *__fmtp++ = '.';
        *__fmtp++ = '*';
    }
    while(*__len)
        *__fmtp++ = *__len++;
    if (floatfield == ios_base::fixed)
    {
        if (uppercase)
            *__fmtp = 'F';
        else
            *__fmtp = 'f';
    }
    else if (floatfield == ios_base::scientific)
    {
        if (uppercase)
            *__fmtp = 'E';
        else
            *__fmtp = 'e';
    }
    else if (floatfield == (ios_base::fixed | ios_base::scientific))
    {
        if (uppercase)
            *__fmtp = 'A';
        else
            *__fmtp = 'a';
    }
    else
    {
        if (uppercase)
            *__fmtp = 'G';
        else
            *__fmtp = 'g';
    }
    return specify_precision;
}

char*
__num_put_base::__identify_padding(char* __nb, char* __ne,
                                   const ios_base& __iob)
{
    switch (__iob.flags() & ios_base::adjustfield)
    {
    case ios_base::internal:
        if (__nb[0] == '-' || __nb[0] == '+')
            return __nb+1;
        if (__ne - __nb >= 2 && __nb[0] == '0'
                            && (__nb[1] == 'x' || __nb[1] == 'X'))
            return __nb+2;
        break;
    case ios_base::left:
        return __ne;
    case ios_base::right:
    default:
        break;
    }
    return __nb;
}

// time_get

static
string*
init_weeks()
{
    static string weeks[14];
    weeks[0]  = "Sunday";
    weeks[1]  = "Monday";
    weeks[2]  = "Tuesday";
    weeks[3]  = "Wednesday";
    weeks[4]  = "Thursday";
    weeks[5]  = "Friday";
    weeks[6]  = "Saturday";
    weeks[7]  = "Sun";
    weeks[8]  = "Mon";
    weeks[9]  = "Tue";
    weeks[10] = "Wed";
    weeks[11] = "Thu";
    weeks[12] = "Fri";
    weeks[13] = "Sat";
    return weeks;
}

static
wstring*
init_wweeks()
{
    static wstring weeks[14];
    weeks[0]  = L"Sunday";
    weeks[1]  = L"Monday";
    weeks[2]  = L"Tuesday";
    weeks[3]  = L"Wednesday";
    weeks[4]  = L"Thursday";
    weeks[5]  = L"Friday";
    weeks[6]  = L"Saturday";
    weeks[7]  = L"Sun";
    weeks[8]  = L"Mon";
    weeks[9]  = L"Tue";
    weeks[10] = L"Wed";
    weeks[11] = L"Thu";
    weeks[12] = L"Fri";
    weeks[13] = L"Sat";
    return weeks;
}

template <>
const string*
__time_get_c_storage<char>::__weeks() const
{
    static const string* weeks = init_weeks();
    return weeks;
}

template <>
const wstring*
__time_get_c_storage<wchar_t>::__weeks() const
{
    static const wstring* weeks = init_wweeks();
    return weeks;
}

static
string*
init_months()
{
    static string months[24];
    months[0]  = "January";
    months[1]  = "February";
    months[2]  = "March";
    months[3]  = "April";
    months[4]  = "May";
    months[5]  = "June";
    months[6]  = "July";
    months[7]  = "August";
    months[8]  = "September";
    months[9]  = "October";
    months[10] = "November";
    months[11] = "December";
    months[12] = "Jan";
    months[13] = "Feb";
    months[14] = "Mar";
    months[15] = "Apr";
    months[16] = "May";
    months[17] = "Jun";
    months[18] = "Jul";
    months[19] = "Aug";
    months[20] = "Sep";
    months[21] = "Oct";
    months[22] = "Nov";
    months[23] = "Dec";
    return months;
}

static
wstring*
init_wmonths()
{
    static wstring months[24];
    months[0]  = L"January";
    months[1]  = L"February";
    months[2]  = L"March";
    months[3]  = L"April";
    months[4]  = L"May";
    months[5]  = L"June";
    months[6]  = L"July";
    months[7]  = L"August";
    months[8]  = L"September";
    months[9]  = L"October";
    months[10] = L"November";
    months[11] = L"December";
    months[12] = L"Jan";
    months[13] = L"Feb";
    months[14] = L"Mar";
    months[15] = L"Apr";
    months[16] = L"May";
    months[17] = L"Jun";
    months[18] = L"Jul";
    months[19] = L"Aug";
    months[20] = L"Sep";
    months[21] = L"Oct";
    months[22] = L"Nov";
    months[23] = L"Dec";
    return months;
}

template <>
const string*
__time_get_c_storage<char>::__months() const
{
    static const string* months = init_months();
    return months;
}

template <>
const wstring*
__time_get_c_storage<wchar_t>::__months() const
{
    static const wstring* months = init_wmonths();
    return months;
}

static
string*
init_am_pm()
{
    static string am_pm[24];
    am_pm[0]  = "AM";
    am_pm[1]  = "PM";
    return am_pm;
}

static
wstring*
init_wam_pm()
{
    static wstring am_pm[24];
    am_pm[0]  = L"AM";
    am_pm[1]  = L"PM";
    return am_pm;
}

template <>
const string*
__time_get_c_storage<char>::__am_pm() const
{
    static const string* am_pm = init_am_pm();
    return am_pm;
}

template <>
const wstring*
__time_get_c_storage<wchar_t>::__am_pm() const
{
    static const wstring* am_pm = init_wam_pm();
    return am_pm;
}

template <>
const string&
__time_get_c_storage<char>::__x() const
{
    static string s("%m/%d/%y");
    return s;
}

template <>
const wstring&
__time_get_c_storage<wchar_t>::__x() const
{
    static wstring s(L"%m/%d/%y");
    return s;
}

template <>
const string&
__time_get_c_storage<char>::__X() const
{
    static string s("%H:%M:%S");
    return s;
}

template <>
const wstring&
__time_get_c_storage<wchar_t>::__X() const
{
    static wstring s(L"%H:%M:%S");
    return s;
}

template <>
const string&
__time_get_c_storage<char>::__c() const
{
    static string s("%a %b %d %H:%M:%S %Y");
    return s;
}

template <>
const wstring&
__time_get_c_storage<wchar_t>::__c() const
{
    static wstring s(L"%a %b %d %H:%M:%S %Y");
    return s;
}

template <>
const string&
__time_get_c_storage<char>::__r() const
{
    static string s("%I:%M:%S %p");
    return s;
}

template <>
const wstring&
__time_get_c_storage<wchar_t>::__r() const
{
    static wstring s(L"%I:%M:%S %p");
    return s;
}

// time_get_byname

__time_get::__time_get(const char* nm)
    : __loc_(newlocale(LC_ALL_MASK, nm, 0))
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__loc_ == 0)
        throw runtime_error("time_get_byname"
                            " failed to construct for " + string(nm));
#endif  // _LIBCPP_NO_EXCEPTIONS
}

__time_get::__time_get(const string& nm)
    : __loc_(newlocale(LC_ALL_MASK, nm.c_str(), 0))
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__loc_ == 0)
        throw runtime_error("time_get_byname"
                            " failed to construct for " + nm);
#endif  // _LIBCPP_NO_EXCEPTIONS
}

__time_get::~__time_get()
{
    freelocale(__loc_);
}

template <>
string
__time_get_storage<char>::__analyze(char fmt, const ctype<char>& ct)
{
    tm t;
    t.tm_sec = 59;
    t.tm_min = 55;
    t.tm_hour = 23;
    t.tm_mday = 31;
    t.tm_mon = 11;
    t.tm_year = 161;
    t.tm_wday = 6;
    t.tm_yday = 364;
    t.tm_isdst = -1;
    char buf[100];
    char f[3] = {0};
    f[0] = '%';
    f[1] = fmt;
    size_t n = strftime_l(buf, 100, f, &t, __loc_);
    char* bb = buf;
    char* be = buf + n;
    string result;
    while (bb != be)
    {
        if (ct.is(ctype_base::space, *bb))
        {
            result.push_back(' ');
            for (++bb; bb != be && ct.is(ctype_base::space, *bb); ++bb)
                ;
            continue;
        }
        char* w = bb;
        ios_base::iostate err = ios_base::goodbit;
        int i = __scan_keyword(w, be, this->__weeks_, this->__weeks_+14,
                               ct, err, false)
                               - this->__weeks_;
        if (i < 14)
        {
            result.push_back('%');
            if (i < 7)
                result.push_back('A');
            else
                result.push_back('a');
            bb = w;
            continue;
        }
        w = bb;
        i = __scan_keyword(w, be, this->__months_, this->__months_+24,
                           ct, err, false)
                           - this->__months_;
        if (i < 24)
        {
            result.push_back('%');
            if (i < 12)
                result.push_back('B');
            else
                result.push_back('b');
            if (fmt == 'x' && ct.is(ctype_base::digit, this->__months_[i][0]))
                result.back() = 'm';
            bb = w;
            continue;
        }
        if (this->__am_pm_[0].size() + this->__am_pm_[1].size() > 0)
        {
            w = bb;
            i = __scan_keyword(w, be, this->__am_pm_, this->__am_pm_+2,
                               ct, err, false) - this->__am_pm_;
            if (i < 2)
            {
                result.push_back('%');
                result.push_back('p');
                bb = w;
                continue;
            }
        }
        w = bb;
        if (ct.is(ctype_base::digit, *bb))
        {
            switch(__get_up_to_n_digits(bb, be, err, ct, 4))
            {
            case 6:
                result.push_back('%');
                result.push_back('w');
                break;
            case 7:
                result.push_back('%');
                result.push_back('u');
                break;
            case 11:
                result.push_back('%');
                result.push_back('I');
                break;
            case 12:
                result.push_back('%');
                result.push_back('m');
                break;
            case 23:
                result.push_back('%');
                result.push_back('H');
                break;
            case 31:
                result.push_back('%');
                result.push_back('d');
                break;
            case 55:
                result.push_back('%');
                result.push_back('M');
                break;
            case 59:
                result.push_back('%');
                result.push_back('S');
                break;
            case 61:
                result.push_back('%');
                result.push_back('y');
                break;
            case 364:
                result.push_back('%');
                result.push_back('j');
                break;
            case 2061:
                result.push_back('%');
                result.push_back('Y');
                break;
            default:
                for (; w != bb; ++w)
                    result.push_back(*w);
                break;
            }
            continue;
        }
        if (*bb == '%')
        {
            result.push_back('%');
            result.push_back('%');
            ++bb;
            continue;
        }
        result.push_back(*bb);
        ++bb;
    }
    return result;
}

template <>
wstring
__time_get_storage<wchar_t>::__analyze(char fmt, const ctype<wchar_t>& ct)
{
    tm t;
    t.tm_sec = 59;
    t.tm_min = 55;
    t.tm_hour = 23;
    t.tm_mday = 31;
    t.tm_mon = 11;
    t.tm_year = 161;
    t.tm_wday = 6;
    t.tm_yday = 364;
    t.tm_isdst = -1;
    char buf[100];
    char f[3] = {0};
    f[0] = '%';
    f[1] = fmt;
    size_t be = strftime_l(buf, 100, f, &t, __loc_);
    wchar_t wbuf[100];
    wchar_t* wbb = wbuf;
    mbstate_t mb = {0};
    const char* bb = buf;
#ifdef _LIBCPP_STABLE_APPLE_ABI
    size_t i = mbsrtowcs_l( wbb, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, __loc_);
#else
    size_t i = __mbsrtowcs_l( wbb, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, __loc_);
#endif
    if (i == -1)
        __throw_runtime_error("locale not supported");
    wchar_t* wbe = wbb + i;
    wstring result;
    while (wbb != wbe)
    {
        if (ct.is(ctype_base::space, *wbb))
        {
            result.push_back(L' ');
            for (++wbb; wbb != wbe && ct.is(ctype_base::space, *wbb); ++wbb)
                ;
            continue;
        }
        wchar_t* w = wbb;
        ios_base::iostate err = ios_base::goodbit;
        int i = __scan_keyword(w, wbe, this->__weeks_, this->__weeks_+14,
                               ct, err, false)
                               - this->__weeks_;
        if (i < 14)
        {
            result.push_back(L'%');
            if (i < 7)
                result.push_back(L'A');
            else
                result.push_back(L'a');
            wbb = w;
            continue;
        }
        w = wbb;
        i = __scan_keyword(w, wbe, this->__months_, this->__months_+24,
                           ct, err, false)
                           - this->__months_;
        if (i < 24)
        {
            result.push_back(L'%');
            if (i < 12)
                result.push_back(L'B');
            else
                result.push_back(L'b');
            if (fmt == 'x' && ct.is(ctype_base::digit, this->__months_[i][0]))
                result.back() = L'm';
            wbb = w;
            continue;
        }
        if (this->__am_pm_[0].size() + this->__am_pm_[1].size() > 0)
        {
            w = wbb;
            i = __scan_keyword(w, wbe, this->__am_pm_, this->__am_pm_+2,
                               ct, err, false) - this->__am_pm_;
            if (i < 2)
            {
                result.push_back(L'%');
                result.push_back(L'p');
                wbb = w;
                continue;
            }
        }
        w = wbb;
        if (ct.is(ctype_base::digit, *wbb))
        {
            switch(__get_up_to_n_digits(wbb, wbe, err, ct, 4))
            {
            case 6:
                result.push_back(L'%');
                result.push_back(L'w');
                break;
            case 7:
                result.push_back(L'%');
                result.push_back(L'u');
                break;
            case 11:
                result.push_back(L'%');
                result.push_back(L'I');
                break;
            case 12:
                result.push_back(L'%');
                result.push_back(L'm');
                break;
            case 23:
                result.push_back(L'%');
                result.push_back(L'H');
                break;
            case 31:
                result.push_back(L'%');
                result.push_back(L'd');
                break;
            case 55:
                result.push_back(L'%');
                result.push_back(L'M');
                break;
            case 59:
                result.push_back(L'%');
                result.push_back(L'S');
                break;
            case 61:
                result.push_back(L'%');
                result.push_back(L'y');
                break;
            case 364:
                result.push_back(L'%');
                result.push_back(L'j');
                break;
            case 2061:
                result.push_back(L'%');
                result.push_back(L'Y');
                break;
            default:
                for (; w != wbb; ++w)
                    result.push_back(*w);
                break;
            }
            continue;
        }
        if (ct.narrow(*wbb, 0) == '%')
        {
            result.push_back(L'%');
            result.push_back(L'%');
            ++wbb;
            continue;
        }
        result.push_back(*wbb);
        ++wbb;
    }
    return result;
}

template <>
void
__time_get_storage<char>::init(const ctype<char>& ct)
{
    tm t;
    char buf[100];
    // __weeks_
    for (int i = 0; i < 7; ++i)
    {
        t.tm_wday = i;
        strftime_l(buf, 100, "%A", &t, __loc_);
        __weeks_[i] = buf;
        strftime_l(buf, 100, "%a", &t, __loc_);
        __weeks_[i+7] = buf;
    }
    // __months_
    for (int i = 0; i < 12; ++i)
    {
        t.tm_mon = i;
        strftime_l(buf, 100, "%B", &t, __loc_);
        __months_[i] = buf;
        strftime_l(buf, 100, "%b", &t, __loc_);
        __months_[i+12] = buf;
    }
    // __am_pm_
    t.tm_hour = 1;
    strftime_l(buf, 100, "%p", &t, __loc_);
    __am_pm_[0] = buf;
    t.tm_hour = 13;
    strftime_l(buf, 100, "%p", &t, __loc_);
    __am_pm_[1] = buf;
    __c_ = __analyze('c', ct);
    __r_ = __analyze('r', ct);
    __x_ = __analyze('x', ct);
    __X_ = __analyze('X', ct);
}

template <>
void
__time_get_storage<wchar_t>::init(const ctype<wchar_t>& ct)
{
    tm t = {0};
    char buf[100];
    size_t be;
    wchar_t wbuf[100];
    wchar_t* wbe;
    mbstate_t mb = {0};
    // __weeks_
    for (int i = 0; i < 7; ++i)
    {
        t.tm_wday = i;
        be = strftime_l(buf, 100, "%A", &t, __loc_);
        mb = mbstate_t();
        const char* bb = buf;
#ifdef _LIBCPP_STABLE_APPLE_ABI
        size_t j = mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, __loc_);
#else
        size_t j = __mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, __loc_);
#endif
        if (j == -1)
            __throw_runtime_error("locale not supported");
        wbe = wbuf + j;
        __weeks_[i].assign(wbuf, wbe);
        be = strftime_l(buf, 100, "%a", &t, __loc_);
        mb = mbstate_t();
        bb = buf;
#ifdef _LIBCPP_STABLE_APPLE_ABI
        j = mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, __loc_);
#else
        j = __mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, __loc_);
#endif
        if (j == -1)
            __throw_runtime_error("locale not supported");
        wbe = wbuf + j;
        __weeks_[i+7].assign(wbuf, wbe);
    }
    // __months_
    for (int i = 0; i < 12; ++i)
    {
        t.tm_mon = i;
        be = strftime_l(buf, 100, "%B", &t, __loc_);
        mb = mbstate_t();
        const char* bb = buf;
#ifdef _LIBCPP_STABLE_APPLE_ABI
        size_t j = mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, __loc_);
#else
        size_t j = __mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, __loc_);
#endif
        if (j == -1)
            __throw_runtime_error("locale not supported");
        wbe = wbuf + j;
        __months_[i].assign(wbuf, wbe);
        be = strftime_l(buf, 100, "%b", &t, __loc_);
        mb = mbstate_t();
        bb = buf;
#ifdef _LIBCPP_STABLE_APPLE_ABI
        j = mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, __loc_);
#else
        j = __mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, __loc_);
#endif
        if (j == -1)
            __throw_runtime_error("locale not supported");
        wbe = wbuf + j;
        __months_[i+12].assign(wbuf, wbe);
    }
    // __am_pm_
    t.tm_hour = 1;
    be = strftime_l(buf, 100, "%p", &t, __loc_);
    mb = mbstate_t();
    const char* bb = buf;
#ifdef _LIBCPP_STABLE_APPLE_ABI
    size_t j = mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, __loc_);
#else
    size_t j = __mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, __loc_);
#endif
    if (j == -1)
        __throw_runtime_error("locale not supported");
    wbe = wbuf + j;
    __am_pm_[0].assign(wbuf, wbe);
    t.tm_hour = 13;
    be = strftime_l(buf, 100, "%p", &t, __loc_);
    mb = mbstate_t();
    bb = buf;
#ifdef _LIBCPP_STABLE_APPLE_ABI
    j = mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, __loc_);
#else
    j = __mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, __loc_);
#endif
    if (j == -1)
        __throw_runtime_error("locale not supported");
    wbe = wbuf + j;
    __am_pm_[1].assign(wbuf, wbe);
    __c_ = __analyze('c', ct);
    __r_ = __analyze('r', ct);
    __x_ = __analyze('x', ct);
    __X_ = __analyze('X', ct);
}

template <class CharT>
struct _LIBCPP_HIDDEN __time_get_temp
    : public ctype_byname<CharT>
{
    explicit __time_get_temp(const char* nm)
        : ctype_byname<CharT>(nm, 1) {}
    explicit __time_get_temp(const string& nm)
        : ctype_byname<CharT>(nm, 1) {}
};

template <>
__time_get_storage<char>::__time_get_storage(const char* __nm)
    : __time_get(__nm)
{
    const __time_get_temp<char> ct(__nm);
    init(ct);
}

template <>
__time_get_storage<char>::__time_get_storage(const string& __nm)
    : __time_get(__nm)
{
    const __time_get_temp<char> ct(__nm);
    init(ct);
}

template <>
__time_get_storage<wchar_t>::__time_get_storage(const char* __nm)
    : __time_get(__nm)
{
    const __time_get_temp<wchar_t> ct(__nm);
    init(ct);
}

template <>
__time_get_storage<wchar_t>::__time_get_storage(const string& __nm)
    : __time_get(__nm)
{
    const __time_get_temp<wchar_t> ct(__nm);
    init(ct);
}

template <>
time_base::dateorder
__time_get_storage<char>::__do_date_order() const
{
    unsigned i;
    for (i = 0; i < __x_.size(); ++i)
        if (__x_[i] == '%')
            break;
    ++i;
    switch (__x_[i])
    {
    case 'y':
    case 'Y':
        for (++i; i < __x_.size(); ++i)
            if (__x_[i] == '%')
                break;
        if (i == __x_.size())
            break;
        ++i;
        switch (__x_[i])
        {
        case 'm':
            for (++i; i < __x_.size(); ++i)
                if (__x_[i] == '%')
                    break;
            if (i == __x_.size())
                break;
            ++i;
            if (__x_[i] == 'd')
                return time_base::ymd;
            break;
        case 'd':
            for (++i; i < __x_.size(); ++i)
                if (__x_[i] == '%')
                    break;
            if (i == __x_.size())
                break;
            ++i;
            if (__x_[i] == 'm')
                return time_base::ydm;
            break;
        }
        break;
    case 'm':
        for (++i; i < __x_.size(); ++i)
            if (__x_[i] == '%')
                break;
        if (i == __x_.size())
            break;
        ++i;
        if (__x_[i] == 'd')
        {
            for (++i; i < __x_.size(); ++i)
                if (__x_[i] == '%')
                    break;
            if (i == __x_.size())
                break;
            ++i;
            if (__x_[i] == 'y' || __x_[i] == 'Y')
                return time_base::mdy;
            break;
        }
        break;
    case 'd':
        for (++i; i < __x_.size(); ++i)
            if (__x_[i] == '%')
                break;
        if (i == __x_.size())
            break;
        ++i;
        if (__x_[i] == 'm')
        {
            for (++i; i < __x_.size(); ++i)
                if (__x_[i] == '%')
                    break;
            if (i == __x_.size())
                break;
            ++i;
            if (__x_[i] == 'y' || __x_[i] == 'Y')
                return time_base::dmy;
            break;
        }
        break;
    }
    return time_base::no_order;
}

template <>
time_base::dateorder
__time_get_storage<wchar_t>::__do_date_order() const
{
    unsigned i;
    for (i = 0; i < __x_.size(); ++i)
        if (__x_[i] == L'%')
            break;
    ++i;
    switch (__x_[i])
    {
    case L'y':
    case L'Y':
        for (++i; i < __x_.size(); ++i)
            if (__x_[i] == L'%')
                break;
        if (i == __x_.size())
            break;
        ++i;
        switch (__x_[i])
        {
        case L'm':
            for (++i; i < __x_.size(); ++i)
                if (__x_[i] == L'%')
                    break;
            if (i == __x_.size())
                break;
            ++i;
            if (__x_[i] == L'd')
                return time_base::ymd;
            break;
        case L'd':
            for (++i; i < __x_.size(); ++i)
                if (__x_[i] == L'%')
                    break;
            if (i == __x_.size())
                break;
            ++i;
            if (__x_[i] == L'm')
                return time_base::ydm;
            break;
        }
        break;
    case L'm':
        for (++i; i < __x_.size(); ++i)
            if (__x_[i] == L'%')
                break;
        if (i == __x_.size())
            break;
        ++i;
        if (__x_[i] == L'd')
        {
            for (++i; i < __x_.size(); ++i)
                if (__x_[i] == L'%')
                    break;
            if (i == __x_.size())
                break;
            ++i;
            if (__x_[i] == L'y' || __x_[i] == L'Y')
                return time_base::mdy;
            break;
        }
        break;
    case L'd':
        for (++i; i < __x_.size(); ++i)
            if (__x_[i] == L'%')
                break;
        if (i == __x_.size())
            break;
        ++i;
        if (__x_[i] == L'm')
        {
            for (++i; i < __x_.size(); ++i)
                if (__x_[i] == L'%')
                    break;
            if (i == __x_.size())
                break;
            ++i;
            if (__x_[i] == L'y' || __x_[i] == L'Y')
                return time_base::dmy;
            break;
        }
        break;
    }
    return time_base::no_order;
}

// time_put

__time_put::__time_put(const char* nm)
    : __loc_(newlocale(LC_ALL_MASK, nm, 0))
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__loc_ == 0)
        throw runtime_error("time_put_byname"
                            " failed to construct for " + string(nm));
#endif  // _LIBCPP_NO_EXCEPTIONS
}

__time_put::__time_put(const string& nm)
    : __loc_(newlocale(LC_ALL_MASK, nm.c_str(), 0))
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__loc_ == 0)
        throw runtime_error("time_put_byname"
                            " failed to construct for " + nm);
#endif  // _LIBCPP_NO_EXCEPTIONS
}

__time_put::~__time_put()
{
    if (__loc_)
        freelocale(__loc_);
}

void
__time_put::__do_put(char* __nb, char*& __ne, const tm* __tm,
                     char __fmt, char __mod) const
{
    char fmt[] = {'%', __fmt, __mod, 0};
    if (__mod != 0)
        swap(fmt[1], fmt[2]);
    size_t n = strftime_l(__nb, __ne-__nb, fmt, __tm, __loc_);
    __ne = __nb + n;
}

void
__time_put::__do_put(wchar_t* __wb, wchar_t*& __we, const tm* __tm,
                     char __fmt, char __mod) const
{
    char __nar[100];
    char* __ne = __nar + 100;
    __do_put(__nar, __ne, __tm, __fmt, __mod);
    mbstate_t mb = {0};
    const char* __nb = __nar;
#ifdef _LIBCPP_STABLE_APPLE_ABI
    size_t j = mbsrtowcs_l(__wb, &__nb, 100, &mb, __loc_);
#else
    size_t j = __mbsrtowcs_l(__wb, &__nb, 100, &mb, __loc_);
#endif
    if (j == -1)
        __throw_runtime_error("locale not supported");
    __we = __wb + j;
}

// moneypunct_byname

static
void
__init_pat(money_base::pattern& pat, char cs_precedes, char sep_by_space, char sign_posn)
{
    const char sign = static_cast<char>(money_base::sign);
    const char space = static_cast<char>(money_base::space);
    const char none = static_cast<char>(money_base::none);
    const char symbol = static_cast<char>(money_base::symbol);
    const char value = static_cast<char>(money_base::value);
    switch (cs_precedes)
    {
    case 0:
        switch (sign_posn)
        {
        case 0:
            pat.field[0] = sign;
            pat.field[1] = value;
            pat.field[3] = symbol;
            switch (sep_by_space)
            {
            case 0:
                pat.field[2] = none;
                return;
            case 1:
            case 2:
                pat.field[2] = space;
                return;
            default:
                break;
            }
            break;
        case 1:
            pat.field[0] = sign;
            pat.field[3] = symbol;
            switch (sep_by_space)
            {
            case 0:
                pat.field[1] = value;
                pat.field[2] = none;
                return;
            case 1:
                pat.field[1] = value;
                pat.field[2] = space;
                return;
            case 2:
                pat.field[1] = space;
                pat.field[2] = value;
                return;
            default:
                break;
            }
            break;
        case 2:
            pat.field[0] = value;
            pat.field[3] = sign;
            switch (sep_by_space)
            {
            case 0:
                pat.field[1] = none;
                pat.field[2] = symbol;
                return;
            case 1:
                pat.field[1] = space;
                pat.field[2] = symbol;
                return;
            case 2:
                pat.field[1] = symbol;
                pat.field[2] = space;
                return;
            default:
                break;
            }
            break;
        case 3:
            pat.field[0] = value;
            pat.field[3] = symbol;
            switch (sep_by_space)
            {
            case 0:
                pat.field[1] = none;
                pat.field[2] = sign;
                return;
            case 1:
                pat.field[1] = space;
                pat.field[2] = sign;
                return;
            case 2:
                pat.field[1] = sign;
                pat.field[2] = space;
                return;
            default:
                break;
            }
            break;
        case 4:
            pat.field[0] = value;
            pat.field[3] = sign;
            switch (sep_by_space)
            {
            case 0:
                pat.field[1] = none;
                pat.field[2] = symbol;
                return;
            case 1:
                pat.field[1] = space;
                pat.field[2] = symbol;
                return;
            case 2:
                pat.field[1] = symbol;
                pat.field[2] = space;
                return;
            default:
                break;
            }
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (sign_posn)
        {
        case 0:
            pat.field[0] = sign;
            pat.field[1] = symbol;
            pat.field[3] = value;
            switch (sep_by_space)
            {
            case 0:
                pat.field[2] = none;
                return;
            case 1:
            case 2:
                pat.field[2] = space;
                return;
            default:
                break;
            }
            break;
        case 1:
            pat.field[0] = sign;
            pat.field[3] = value;
            switch (sep_by_space)
            {
            case 0:
                pat.field[1] = symbol;
                pat.field[2] = none;
                return;
            case 1:
                pat.field[1] = symbol;
                pat.field[2] = space;
                return;
            case 2:
                pat.field[1] = space;
                pat.field[2] = symbol;
                return;
            default:
                break;
            }
            break;
        case 2:
            pat.field[0] = symbol;
            pat.field[3] = sign;
            switch (sep_by_space)
            {
            case 0:
                pat.field[1] = none;
                pat.field[2] = value;
                return;
            case 1:
                pat.field[1] = space;
                pat.field[2] = value;
                return;
            case 2:
                pat.field[1] = value;
                pat.field[2] = space;
                return;
            default:
                break;
            }
            break;
        case 3:
            pat.field[0] = sign;
            pat.field[3] = value;
            switch (sep_by_space)
            {
            case 0:
                pat.field[1] = symbol;
                pat.field[2] = none;
                return;
            case 1:
                pat.field[1] = symbol;
                pat.field[2] = space;
                return;
            case 2:
                pat.field[1] = space;
                pat.field[2] = symbol;
                return;
            default:
                break;
            }
            break;
        case 4:
            pat.field[0] = symbol;
            pat.field[3] = value;
            switch (sep_by_space)
            {
            case 0:
                pat.field[1] = sign;
                pat.field[2] = none;
                return;
            case 1:
                pat.field[1] = sign;
                pat.field[2] = space;
                return;
            case 2:
                pat.field[1] = space;
                pat.field[2] = sign;
                return;
           default:
                break;
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    pat.field[0] = symbol;
    pat.field[1] = sign;
    pat.field[2] = none;
    pat.field[3] = value;
}

template<>
void
moneypunct_byname<char, false>::init(const char* nm)
{
    typedef moneypunct<char, false> base;
    __locale_unique_ptr loc(newlocale(LC_ALL_MASK, nm, 0), freelocale);
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (loc == 0)
        throw runtime_error("moneypunct_byname"
                            " failed to construct for " + string(nm));
#endif  // _LIBCPP_NO_EXCEPTIONS
#ifdef _LIBCPP_STABLE_APPLE_ABI
    lconv* lc = localeconv_l(loc.get());
#else
    lconv* lc = __localeconv_l(loc.get());
#endif
    if (*lc->mon_decimal_point)
        __decimal_point_ = *lc->mon_decimal_point;
    else
        __decimal_point_ = base::do_decimal_point();
    if (*lc->mon_thousands_sep)
        __thousands_sep_ = *lc->mon_thousands_sep;
    else
        __thousands_sep_ = base::do_thousands_sep();
    __grouping_ = lc->mon_grouping;
    __curr_symbol_ = lc->currency_symbol;
    if (lc->frac_digits != CHAR_MAX)
        __frac_digits_ = lc->frac_digits;
    else
        __frac_digits_ = base::do_frac_digits();
    if (lc->p_sign_posn == 0)
        __positive_sign_ = "()";
    else
        __positive_sign_ = lc->positive_sign;
    if (lc->n_sign_posn == 0)
        __negative_sign_ = "()";
    else
        __negative_sign_ = lc->negative_sign;
    __init_pat(__pos_format_, lc->p_cs_precedes, lc->p_sep_by_space, lc->p_sign_posn);
    __init_pat(__neg_format_, lc->n_cs_precedes, lc->n_sep_by_space, lc->n_sign_posn);
}

template<>
void
moneypunct_byname<char, true>::init(const char* nm)
{
    typedef moneypunct<char, true> base;
    __locale_unique_ptr loc(newlocale(LC_ALL_MASK, nm, 0), freelocale);
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (loc == 0)
        throw runtime_error("moneypunct_byname"
                            " failed to construct for " + string(nm));
#endif  // _LIBCPP_NO_EXCEPTIONS
#ifdef _LIBCPP_STABLE_APPLE_ABI
    lconv* lc = localeconv_l(loc.get());
#else
    lconv* lc = __localeconv_l(loc.get());
#endif
    if (*lc->mon_decimal_point)
        __decimal_point_ = *lc->mon_decimal_point;
    else
        __decimal_point_ = base::do_decimal_point();
    if (*lc->mon_thousands_sep)
        __thousands_sep_ = *lc->mon_thousands_sep;
    else
        __thousands_sep_ = base::do_thousands_sep();
    __grouping_ = lc->mon_grouping;
    __curr_symbol_ = lc->int_curr_symbol;
    if (lc->int_frac_digits != CHAR_MAX)
        __frac_digits_ = lc->int_frac_digits;
    else
        __frac_digits_ = base::do_frac_digits();
    if (lc->int_p_sign_posn == 0)
        __positive_sign_ = "()";
    else
        __positive_sign_ = lc->positive_sign;
    if (lc->int_n_sign_posn == 0)
        __negative_sign_ = "()";
    else
        __negative_sign_ = lc->negative_sign;
    __init_pat(__pos_format_, lc->int_p_cs_precedes, lc->int_p_sep_by_space, lc->int_p_sign_posn);
    __init_pat(__neg_format_, lc->int_n_cs_precedes, lc->int_n_sep_by_space, lc->int_n_sign_posn);
}

template<>
void
moneypunct_byname<wchar_t, false>::init(const char* nm)
{
    typedef moneypunct<wchar_t, false> base;
    __locale_unique_ptr loc(newlocale(LC_ALL_MASK, nm, 0), freelocale);
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (loc == 0)
        throw runtime_error("moneypunct_byname"
                            " failed to construct for " + string(nm));
#endif  // _LIBCPP_NO_EXCEPTIONS
#ifdef _LIBCPP_STABLE_APPLE_ABI
    lconv* lc = localeconv_l(loc.get());
#else
    lconv* lc = __localeconv_l(loc.get());
#endif
    if (*lc->mon_decimal_point)
        __decimal_point_ = static_cast<wchar_t>(*lc->mon_decimal_point);
    else
        __decimal_point_ = base::do_decimal_point();
    if (*lc->mon_thousands_sep)
        __thousands_sep_ = static_cast<wchar_t>(*lc->mon_thousands_sep);
    else
        __thousands_sep_ = base::do_thousands_sep();
    __grouping_ = lc->mon_grouping;
    wchar_t wbuf[100];
    mbstate_t mb = {0};
    const char* bb = lc->currency_symbol;
#ifdef _LIBCPP_STABLE_APPLE_ABI
    size_t j = mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, loc.get());
#else
    size_t j = __mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, loc.get());
#endif
    if (j == -1)
        __throw_runtime_error("locale not supported");
    wchar_t* wbe = wbuf + j;
    __curr_symbol_.assign(wbuf, wbe);
    if (lc->frac_digits != CHAR_MAX)
        __frac_digits_ = lc->frac_digits;
    else
        __frac_digits_ = base::do_frac_digits();
    if (lc->p_sign_posn == 0)
        __positive_sign_ = L"()";
    else
    {
        mb = mbstate_t();
        bb = lc->positive_sign;
#ifdef _LIBCPP_STABLE_APPLE_ABI
        j = mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, loc.get());
#else
        j = __mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, loc.get());
#endif
        if (j == -1)
            __throw_runtime_error("locale not supported");
        wbe = wbuf + j;
        __positive_sign_.assign(wbuf, wbe);
    }
    if (lc->n_sign_posn == 0)
        __negative_sign_ = L"()";
    else
    {
        mb = mbstate_t();
        bb = lc->negative_sign;
#ifdef _LIBCPP_STABLE_APPLE_ABI
        j = mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, loc.get());
#else
        j = __mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, loc.get());
#endif
        if (j == -1)
            __throw_runtime_error("locale not supported");
        wbe = wbuf + j;
        __negative_sign_.assign(wbuf, wbe);
    }
    __init_pat(__pos_format_, lc->p_cs_precedes, lc->p_sep_by_space, lc->p_sign_posn);
    __init_pat(__neg_format_, lc->n_cs_precedes, lc->n_sep_by_space, lc->n_sign_posn);
}

template<>
void
moneypunct_byname<wchar_t, true>::init(const char* nm)
{
    typedef moneypunct<wchar_t, true> base;
    __locale_unique_ptr loc(newlocale(LC_ALL_MASK, nm, 0), freelocale);
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (loc == 0)
        throw runtime_error("moneypunct_byname"
                            " failed to construct for " + string(nm));
#endif  // _LIBCPP_NO_EXCEPTIONS
#ifdef _LIBCPP_STABLE_APPLE_ABI
    lconv* lc = localeconv_l(loc.get());
#else
    lconv* lc = __localeconv_l(loc.get());
#endif
    if (*lc->mon_decimal_point)
        __decimal_point_ = static_cast<wchar_t>(*lc->mon_decimal_point);
    else
        __decimal_point_ = base::do_decimal_point();
    if (*lc->mon_thousands_sep)
        __thousands_sep_ = static_cast<wchar_t>(*lc->mon_thousands_sep);
    else
        __thousands_sep_ = base::do_thousands_sep();
    __grouping_ = lc->mon_grouping;
    wchar_t wbuf[100];
    mbstate_t mb = {0};
    const char* bb = lc->int_curr_symbol;
#ifdef _LIBCPP_STABLE_APPLE_ABI
    size_t j = mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, loc.get());
#else
    size_t j = __mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, loc.get());
#endif
    if (j == -1)
        __throw_runtime_error("locale not supported");
    wchar_t* wbe = wbuf + j;
    __curr_symbol_.assign(wbuf, wbe);
    if (lc->int_frac_digits != CHAR_MAX)
        __frac_digits_ = lc->int_frac_digits;
    else
        __frac_digits_ = base::do_frac_digits();
    if (lc->int_p_sign_posn == 0)
        __positive_sign_ = L"()";
    else
    {
        mb = mbstate_t();
        bb = lc->positive_sign;
#ifdef _LIBCPP_STABLE_APPLE_ABI
        j = mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, loc.get());
#else
        j = __mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, loc.get());
#endif
        if (j == -1)
            __throw_runtime_error("locale not supported");
        wbe = wbuf + j;
        __positive_sign_.assign(wbuf, wbe);
    }
    if (lc->int_n_sign_posn == 0)
        __negative_sign_ = L"()";
    else
    {
        mb = mbstate_t();
        bb = lc->negative_sign;
#ifdef _LIBCPP_STABLE_APPLE_ABI
        j = mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, loc.get());
#else
        j = __mbsrtowcs_l(wbuf, &bb, sizeof(wbuf)/sizeof(wbuf[0]), &mb, loc.get());
#endif
        if (j == -1)
            __throw_runtime_error("locale not supported");
        wbe = wbuf + j;
        __negative_sign_.assign(wbuf, wbe);
    }
    __init_pat(__pos_format_, lc->int_p_cs_precedes, lc->int_p_sep_by_space, lc->int_p_sign_posn);
    __init_pat(__neg_format_, lc->int_n_cs_precedes, lc->int_n_sep_by_space, lc->int_n_sign_posn);
}

void __do_nothing(void*) {}

void __throw_runtime_error(const char* msg)
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    throw runtime_error(msg);
#endif
}

template class collate<char>;
template class collate<wchar_t>;

template class num_get<char>;
template class num_get<wchar_t>;

template class __num_get<char>;
template class __num_get<wchar_t>;

template class num_put<char>;
template class num_put<wchar_t>;

template class __num_put<char>;
template class __num_put<wchar_t>;

template class time_get<char>;
template class time_get<wchar_t>;

template class time_get_byname<char>;
template class time_get_byname<wchar_t>;

template class time_put<char>;
template class time_put<wchar_t>;

template class time_put_byname<char>;
template class time_put_byname<wchar_t>;

template class moneypunct<char, false>;
template class moneypunct<char, true>;
template class moneypunct<wchar_t, false>;
template class moneypunct<wchar_t, true>;

template class moneypunct_byname<char, false>;
template class moneypunct_byname<char, true>;
template class moneypunct_byname<wchar_t, false>;
template class moneypunct_byname<wchar_t, true>;

template class money_get<char>;
template class money_get<wchar_t>;

template class __money_get<char>;
template class __money_get<wchar_t>;

template class money_put<char>;
template class money_put<wchar_t>;

template class __money_put<char>;
template class __money_put<wchar_t>;

template class messages<char>;
template class messages<wchar_t>;

template class messages_byname<char>;
template class messages_byname<wchar_t>;

template class codecvt_byname<char, char, mbstate_t>;
template class codecvt_byname<wchar_t, char, mbstate_t>;
template class codecvt_byname<char16_t, char, mbstate_t>;
template class codecvt_byname<char32_t, char, mbstate_t>;

template class __vector_base_common<true>;

_LIBCPP_END_NAMESPACE_STD

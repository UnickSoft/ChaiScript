#ifndef __type_info_hpp__
#define __type_info_hpp__

#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/ref.hpp>

namespace dispatchkit
{
  struct Type_Info
  {
    Type_Info(bool t_is_const, bool t_is_reference, bool t_is_pointer, bool t_is_void, 
        const std::type_info *t_ti, const std::type_info *t_bareti)
      : m_is_const(t_is_const), m_is_reference(t_is_reference), m_is_pointer(t_is_pointer),
        m_is_void(t_is_void),
      m_type_info(t_ti), m_bare_type_info(t_bareti),
      m_is_unknown(false)
    {
    }

    Type_Info()
      : m_is_const(false), m_is_reference(false), m_is_pointer(false),
      m_is_void(false), m_type_info(0), m_bare_type_info(0),
      m_is_unknown(true)
    {
    }

    Type_Info(const Type_Info &ti)
    : m_is_const(ti.m_is_const), m_is_reference(ti.m_is_reference), 
      m_is_pointer(ti.m_is_pointer),
      m_is_void(ti.m_is_void), m_type_info(ti.m_type_info), 
      m_bare_type_info(ti.m_bare_type_info),
      m_is_unknown(ti.m_is_unknown)
    {
    }
    Type_Info &operator=(const Type_Info &ti)
    {
      m_is_const = ti.m_is_const;
      m_is_reference = ti.m_is_reference;
      m_is_pointer = ti.m_is_pointer;
      m_is_void = ti.m_is_void;
      m_type_info = ti.m_type_info;
      m_bare_type_info = ti.m_bare_type_info;
      m_is_unknown = ti.m_is_unknown;
      return *this;
    }
    bool operator<(const Type_Info &ti) const
    {
      return m_type_info < ti.m_type_info;
    }

    bool operator==(const Type_Info &ti) const
    {
      return ti.m_type_info == m_type_info;
    }

    bool m_is_const;
    bool m_is_reference;
    bool m_is_pointer;
    bool m_is_void;
    const std::type_info *m_type_info;
    const std::type_info *m_bare_type_info;
    bool m_is_unknown;
  };

  template<typename T>
    struct Get_Type_Info
    {
      static Type_Info get()
      {
        return Type_Info(boost::is_const<T>::value, boost::is_reference<T>::value, boost::is_pointer<T>::value, 
            boost::is_void<T>::value,
            &typeid(T), 
            &typeid(typename boost::remove_const<typename boost::remove_pointer<typename boost::remove_reference<T>::type>::type>::type));
      }
    };

  template<typename T>
    struct Get_Type_Info<boost::shared_ptr<T> >
    {
      static Type_Info get()
      {
        return Type_Info(boost::is_const<T>::value, boost::is_reference<T>::value, boost::is_pointer<T>::value, 
            boost::is_void<T>::value,
            &typeid(boost::shared_ptr<T> ), 
            &typeid(typename boost::remove_const<typename boost::remove_pointer<typename boost::remove_reference<T>::type>::type>::type));
      }
    };

  template<typename T>
    struct Get_Type_Info<boost::reference_wrapper<T> >
    {
      static Type_Info get()
      {
        return Type_Info(boost::is_const<T>::value, boost::is_reference<T>::value, boost::is_pointer<T>::value, 
            boost::is_void<T>::value,
            &typeid(boost::reference_wrapper<T> ), 
            &typeid(typename boost::remove_const<typename boost::remove_pointer<typename boost::remove_reference<T>::type>::type>::type));
      }
    };
}

#endif



#ifndef __HH_ROCKHOPPER_UTIL_CACHE_VARIABLE_
#define __HH_ROCKHOPPER_UTIL_CACHE_VARIABLE_

#include <functional>
#include <type_traits>

namespace RockHopper
{

    template <typename T_DataType>
    class CacheVariable
    {
    public:
        virtual ~CacheVariable() = default;

        template <typename... T_Args>
            requires std::is_constructible<T_DataType,T_Args...>::value
        CacheVariable(T_Args&&... args)
            : m_Data{std::forward<T_Args>(args)...}
        {
        }

        auto get() const -> T_DataType const&
        {
            if (m_HasRecacheFunction && m_IsDirty)
            {
                m_Data = m_RecacheFunction();
                m_IsDirty = false;
            }
            return m_Data;
        }
        auto get() -> T_DataType&
        {
            return const_cast<T_DataType&>(const_cast<const CacheVariable*>(this)->get());
        }

        void set_recache_function(std::function<T_DataType(void)> const& func)
        {
            m_RecacheFunction = func;
            m_HasRecacheFunction = true;
        }
        void set_recache_function(std::function<T_DataType(void)>&& func)
        {
            m_RecacheFunction = std::move(func);
            m_HasRecacheFunction = true;
        }

        void mark_dirty()
        {
            m_IsDirty = true;
        }
        void mark_clean()
        {
            m_IsDirty = false;
        }
        bool is_dirty() const
        {
            return m_IsDirty == true;
        }
        bool is_clean() const
        {
            return m_IsDirty == false;
        }

        auto operator->() const -> T_DataType const&
        {
            return get();
        }
        auto operator->() -> T_DataType&
        {
            return get();
        }
        auto operator*() const -> T_DataType const&
        {
            return get();
        }
        auto operator*() -> T_DataType&
        {
            return get();
        }
        template <typename T_Index>
        auto operator[](T_Index&& index) const
        {
            return m_Data[std::forward<T_Index>(index)];
        }
        template <typename T_Index>
        auto operator[](T_Index&& index)
        {
            return m_Data[std::forward<T_Index>(index)];
        }

    private:
        mutable T_DataType m_Data;
        mutable bool m_IsDirty{};

        std::function<T_DataType(void)> m_RecacheFunction{};
        bool m_HasRecacheFunction{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_UTIL_CACHE_VARIABLE_ */

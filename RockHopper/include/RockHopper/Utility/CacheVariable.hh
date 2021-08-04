
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
        explicit CacheVariable(T_Args&&... args)
            : m_Data{std::forward<T_Args>(args)...}
            , m_IsDirty{true}
        {
        }

        CacheVariable operator=(T_DataType const& data)
        {
            m_Data = data;
            m_IsDirty = false;
        }
        CacheVariable operator=(T_DataType&& data)
        {
            m_Data = std::move(data);
            m_IsDirty = false;
        }

        void set_recalculate_function(std::function<T_DataType(void)> const& func)
        {
            m_Recalculate = func;
        }

        operator T_DataType() const
        {
            return get();
        }
        operator T_DataType()
        {
            return get();
        }

        auto get() const -> T_DataType const&
        {
            if (m_IsDirty)
            {
                m_Data = m_Recalculate();
                m_IsDirty = false;
            }
            return m_Data;
        }
        auto get() -> T_DataType&
        {
            // call the const version
            return const_cast<T_DataType&>(const_cast<const CacheVariable*>(this)->get());
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

    private:
        mutable T_DataType m_Data;
        mutable bool m_IsDirty{};

        std::function<T_DataType(void)> m_Recalculate{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_UTIL_CACHE_VARIABLE_ */

/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "simulator/Configuration.hpp"

// Simulator
#include "core/Map.hpp"
#include "core/String.hpp"
#include "core/StringView.hpp"
#include "core/SharedPtr.hpp"
#include "core/DynamicArray.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Memory configuration implementation.
 */
class MemoryImplementation : public Configuration::Implementation
{

// Private Structures
private:


    /**
     * @brief Data storage.
     */
    struct Data
    {
        /// Stored values.
        Map<String, String> values;

        /// Stored content.
        String content;

        /// Subdata
        Map<String, DynamicArray<SharedPtr<Data>>> data;
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    MemoryImplementation()
        : m_data(makeShared<Data>())
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param data Managed data.
     */
    explicit MemoryImplementation(SharedPtr<Data> data)
        : m_data(std::move(data))
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Check if value with given name exists.
     *
     * @param name Value name.
     *
     * @return
     */
    bool has(const StringView& name) const noexcept override
    {
        return m_data->values.find(name.getData()) != m_data->values.end();
    }


    /**
     * @brief Returns string value.
     *
     * @param name Value name.
     *
     * @return
     */
    String get(const StringView& name) const noexcept override
    {
        auto it = m_data->values.find(name.getData());
        return it != m_data->values.end() ? it->second : "";
    }


    /**
     * @brief Returns list of configuration names.
     *
     * @return
     */
    DynamicArray<String> getNames() const noexcept override
    {
        DynamicArray<String> names;
        names.reserve(m_data->values.size());

        for (const auto& p : m_data->values)
            names.push_back(std::move(p.first));

        return names;
    }


    /**
     * @brief Returns if content string is set.
     *
     * @return
     */
    bool hasContent() const noexcept override
    {
        return !m_data->content.empty();
    }


    /**
     * @brief Returns content string.
     *
     * @return
     */
    String getContent() const noexcept override
    {
        return m_data->content;
    }


    /**
     * @brief Returns if sub-configuration exists.
     *
     * @param name Sub-configuration name.
     *
     * @return
     */
    bool hasSubs(const StringView& name) const noexcept override
    {
        return m_data->data.find(name.getData()) != m_data->data.end();
    }


    /**
     * @brief Returns all sub-configuration with given name.
     *
     * @param name Sub-configuration name.
     *
     * @return
     */
    DynamicArray<UniquePtr<Implementation>> getSubs(const StringView& name) const noexcept override
    {
        DynamicArray<UniquePtr<Implementation>> res;

        auto it = m_data->data.find(name.getData());

        if (it == m_data->data.end())
            return res;

        for (const auto& ptr : it->second)
            res.push_back(makeUnique<MemoryImplementation>(ptr));

        return res;
    }


    /**
     * @brief Returns list of sub-configuration names.
     *
     * @return
     */
    DynamicArray<String> getSubNames() const noexcept override
    {
        DynamicArray<String> names;
        names.reserve(m_data->data.size());

        for (const auto& p : m_data->data)
            names.push_back(p.first);

        return names;
    }


// Public Mutators
public:


    /**
     * @brief Set string value.
     *
     * @param name  Value name.
     * @param value Value to store.
     *
     * @return
     */
    void set(const StringView& name, const StringView& value) noexcept override
    {
        m_data->values[name.getData()] = value.getData();
    }


    /**
     * @brief Store content.
     *
     * @param content Content text.
     */
    void setContent(const StringView& content) noexcept override
    {
        m_data->content = content.getData();
    }


    /**
     * @brief Create new sub-configuration.
     *
     * @param name Sub-configuration name.
     *
     * @return
     */
    UniquePtr<Implementation> addSub(const StringView& name) noexcept override
    {
        auto data = makeShared<Data>();

        // Register data
        m_data->data[name.getData()].push_back(data);

        return makeUnique<MemoryImplementation>(std::move(data));
    }


// Private Data Members
private:


    /// Shared data.
    SharedPtr<Data> m_data;
};

/* ************************************************************************ */

}

/* ************************************************************************ */

Configuration::Configuration(FilePath path) noexcept
    : m_impl(makeUnique<MemoryImplementation>())
    , m_filePath(std::move(path))
{
    // Nothign to do
}

/* ************************************************************************ */

FilePath Configuration::buildFilePath(const FilePath& filename) const noexcept
{
    // Remove filename
    auto sourceFile = getSourcePath();

    // Return file path
    return sourceFile.parent_path() / filename;
}

/* ************************************************************************ */

void Configuration::copyFrom(const Configuration& config)
{
    for (const auto& name : config.getNames())
        set(name, config.get(name));

    setContent(config.getContent());

    // Copy subconfigurations
    for (const auto& name : config.getConfigurationNames())
    {
        for (auto&& cfg : config.getConfigurations(name))
            addConfiguration(name).copyFrom(cfg);
    }
}

/* ************************************************************************ */

Configuration Configuration::toMemory() const
{
    Configuration config{getSourcePath()};
    config.copyFrom(*this);
    return config;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

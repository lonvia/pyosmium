/* SPDX-License-Identifier: BSD-2-Clause
 *
 * This file is part of pyosmium. (https://osmcode.org/pyosmium/)
 *
 * Copyright (C) 2025 Sarah Hoffmann <lonvia@denofr.de> and others.
 * For a full list of authors see the git log.
 */
#include <pybind11/pybind11.h>
#include <pybind11/stl/filesystem.h>

#include <osmium/osm.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>
#include <osmium/thread/pool.hpp>

#include <filesystem>

#include "cast.h"
#include "io.h"

namespace py = pybind11;

namespace {

struct LastChangeHandler : public osmium::handler::Handler
{
    osmium::Timestamp last_change;

    void osm_object(osmium::OSMObject const &obj)
    {
        if (obj.timestamp() > last_change) {
            last_change = obj.timestamp();
        }
    }
};

osmium::Timestamp newest_change_from_file(osmium::io::Reader &reader) {
    LastChangeHandler handler;
    osmium::apply(reader, handler);
    return handler.last_change;
}

} // namespace

#ifdef Py_GIL_DISABLED
PYBIND11_MODULE(_replication, m, py::mod_gil_not_used())
#else
PYBIND11_MODULE(_replication, m)
#endif
{
    m.def("newest_change_from_file", [](pyosmium::PyReader &reader)
        {
            return newest_change_from_file(*reader.get());
        })
     .def("newest_change_from_file", [](char const *filename)
        {
            osmium::thread::Pool thread_pool{};
            osmium::io::Reader reader{filename, osmium::osm_entity_bits::nwr,
                                      thread_pool};

            return newest_change_from_file(reader);
        })
     .def("newest_change_from_file", [](std::filesystem::path const &file)
        {
            osmium::thread::Pool thread_pool{};
            osmium::io::Reader reader{file.string(), osmium::osm_entity_bits::nwr,
                                      thread_pool};

            return newest_change_from_file(reader);
        });
}

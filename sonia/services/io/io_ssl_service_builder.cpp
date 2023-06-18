//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "io_ssl_service_builder.hpp"

namespace sonia::services {

namespace sp = sonia::parameters;

void io_ssl_service_builder::open()
{
    auto media_type_handler = [](json_value const& v)->io::ssl_media_type {
        auto sv = v.get_string();
        if (sv == "string") return io::ssl_media_type::STRING;
        if (sv == "file") return io::ssl_media_type::FILE;
        throw exception("unknown string value '%1%' for the ssl_media_type"_fmt % sv);
    };
    parameters_.bind()
        .variable("tcp-socket-factory", &io_ssl_service_configuration::tcp_socket_factory, "base tcp socket factory")
        .variable("tcp-server-socket-factory", &io_ssl_service_configuration::tcp_server_socket_factory, "base tcp server socket factory")
        .variable("certificate", &io_ssl_service_configuration::certificate, "ssl certificate").required()
        .variable("certificate-media", &io_ssl_service_configuration::certificate_media_type, "ssl certificate media type").required()
            .binder(media_type_handler)
        .variable("certificate-type", &io_ssl_service_configuration::certificate_type, "ssl certificate type").required()
            .binder([](json_value const& v)->io::x509_format_type {
                auto sv = v.get_string();
                if (sv == "DER") return io::x509_format_type::DERFORMAT;
                if (sv == "PEM") return io::x509_format_type::PEMFORMAT;
                throw exception("unknown string value '%1%' for the io_ssl_certificate_type"_fmt % sv);
            })
        .variable("private-key", &io_ssl_service_configuration::private_key, "ssl private key").required()
        .variable("private-key-password", &io_ssl_service_configuration::private_key_password, "ssl private key password")
        .variable("private-key-media", &io_ssl_service_configuration::private_key_media_type, "ssl private key media type").required()
            .binder(media_type_handler)
        .variable("private-key-type", &io_ssl_service_configuration::private_key_type, "ssl private key type").required()
            .binder([](json_value const& v)->io::key_format_type {
                auto sv = v.get_string();
                if (sv == "RSA") return io::key_format_type::PRIVATE_RSA;
                if (sv == "EVP") return io::key_format_type::PRIVATE_EVP;
                throw exception("unknown string value '%1%' for the private_key_type"_fmt % sv);
            })
    ;
}

}

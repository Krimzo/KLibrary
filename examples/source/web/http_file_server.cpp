#include "examples.h"


namespace fs = std::filesystem;

struct MyFileServer : kl::HttpApp
{
    const std::string root;

    MyFileServer( const std::string root )
        : root( root )
    {
        default_route = []( kl::HttpRequest const& request, kl::HttpQuerry const& querry, kl::HttpResponse& response )
            {
                response.status_code = kl::HttpStatusCode::NOT_FOUND;
                response.load_text( "Does not exist." );
                kl::print( "Requsted not supported: ", request.full_path );
            };

        exact_routes["/"] = []( kl::HttpRequest const& request, kl::HttpQuerry const& querry, kl::HttpResponse& response )
            {
                response.status_code = kl::HttpStatusCode::FOUND;
                response.headers["Location"] = "/explore";
            };

        exact_routes["/index.css"] = []( kl::HttpRequest const& request, kl::HttpQuerry const& querry, kl::HttpResponse& response )
            {
                response.content_type = kl::HttpContentType::CSS;
                response.content = kl::read_file_string( "dist/index.css" );
            };

        starting_routes["/explore"] = [this]( kl::HttpRequest const& request, kl::HttpQuerry const& querry, std::string_view const& path, kl::HttpResponse& response )
            {
                const fs::path fs_path = fs::absolute( this->root + "/" + kl::decode_url_string( path ) );
                if ( !fs::exists( fs_path ) )
                {
                    handle_non_existent( request, querry, path, response, fs_path );
                }
                else if ( fs::is_directory( fs_path ) )
                {
                    handle_view_folder( request, querry, path, response, fs_path );
                }
                else
                {
                    handle_view_file( request, querry, path, response, fs_path );
                }
            };
    }

private:
    void handle_non_existent( kl::HttpRequest const& request, kl::HttpQuerry const& querry, std::string_view const& path, kl::HttpResponse& response, fs::path const& fs_path )
    {
        response.load( "Does not exist: ", path );
    }

    void handle_view_folder( kl::HttpRequest const& request, kl::HttpQuerry const& querry, std::string_view const& path, kl::HttpResponse& response, fs::path const& fs_path )
    {
        const kl::Html folder_item_template = kl::Html::from_file( "dist/folder_item.html" );
        const kl::Html file_item_template = kl::Html::from_file( "dist/file_item.html" );
        const kl::Html thumbnail_template = kl::Html::from_file( "dist/thumbnail_item.html" );

        std::stringstream item_grid_builder;
        for ( auto& entry : fs::directory_iterator{ fs_path } )
        {
            const std::optional opt_type = kl::probe_content_type( entry );
            const std::string encoded_path = fs::relative( entry, root ).generic_string();
            const std::string file_name = fs::path{ entry }.filename().generic_string();
            if ( fs::is_directory( entry ) )
            {
                kl::Html templatee = folder_item_template;
                templatee.apply_var( "url", kl::format( "/explore/", encoded_path ) );
                templatee.apply_var( "name", file_name );
                item_grid_builder << templatee.data << "\n";
            }
            else if ( opt_type && ( opt_type->starts_with( "image" ) || opt_type->starts_with( "video" ) ) )
            {
                kl::Html templatee = thumbnail_template;
                templatee.apply_var( "url", kl::format( "/explore/", encoded_path ) );
                templatee.apply_var( "thumbnail_url", kl::format( "/explore/", encoded_path, "?thumbnail" ) );
                templatee.apply_var( "name", file_name );
                item_grid_builder << templatee.data << "\n";
            }
            else
            {
                kl::Html templatee = file_item_template;
                templatee.apply_var( "url", kl::format( "/explore/", encoded_path ) );
                templatee.apply_var( "name", file_name );
                item_grid_builder << templatee.data << "\n";
            }
        }

        const std::string file_name_str = std::string( fs_path.has_filename() ? kl::format( "My File Server: ", fs_path.filename().generic_string() ) : "My File Server" );
        const std::string file_path_str = std::string( fs_path.has_filename() ? fs::relative( fs_path, root ).generic_string() : "My File Server" );

        kl::Html folder_content_template = kl::Html::from_file( "dist/folder_content.html" );
        folder_content_template.apply_var( "title", file_name_str );
        folder_content_template.apply_var( "folder_path", file_path_str );
        folder_content_template.apply_var( "grid_items", item_grid_builder.str() );

        response.load_html( folder_content_template );
    }

    void handle_view_file( kl::HttpRequest const& request, kl::HttpQuerry const& querry, std::string_view const& path, kl::HttpResponse& response, fs::path const& fs_path )
    {
        if ( querry.contains( "thumbnail" ) && kl::probe_content_type( path ).value_or( {} ).starts_with( "video" ) )
        {
            response.content_type = kl::HttpContentType::JPEG;
            response.content = read_video_frame( fs_path.generic_string() );
        }
        else
            response.load_file( fs_path, request.byte_range() );
    }

    std::string read_video_frame( std::string_view const& path )
    {
        kl::VideoReader reader{ path };
        kl::Image frame;
        reader.read_frame( frame );
        std::string buffer;
        frame.save_to_buffer( buffer, kl::ImageType::JPG );
        return buffer;
    }
};

int examples::http_file_server( int argc, char** argv )
{
    MyFileServer{ "vault" }.start( 80 );
    return 0;
}

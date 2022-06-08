from .app.urls import initialize_routes


def include_urls(api):
    initialize_routes(api)
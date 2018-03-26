/**
 * An abstract type to describe a query string object.
 */
type queryString;

/**
 * The signature of the `graphql-tag/gql` function that transforms a GraphQL
 * query string to the standard GraphQL AST.
 * https://github.com/apollographql/graphql-tag
 */
type gql = (. string) => queryString;

/**
 * An abstract type to describe an Apollo Link object.
 */
type apolloLink;

/**
 * An abstract type to describe an Apollo Cache object.
 */
type apolloCache;

type networkError = {. "statusCode": int};

module NetworkState = {
  type t =
    | Loading
    | SetVariables
    | FetchMore
    | Refetch
    | Poll
    | Ready
    | Error;
  let to_int =
    fun
    | Loading => 1
    | SetVariables => 2
    | FetchMore => 3
    | Refetch => 4
    | Poll => 6
    | Ready => 7
    | Error => 8;
};

type apolloError;

/* TODO: define missing keys */
type apolloLinkErrorResponse = {. "networkError": option(networkError)};
let component = ReasonReact.statelessComponent("Page");

let int_from_opt = (~default: int=0, option: option(int)) =>
  switch (option) {
  | None => default
  | Some(a) => a
  };

module CountQuery = [%graphql
  {|
    query films {
      allFilms {
        totalCount
      }
    }
|}
];

module FilmsQuery = ReactApollo.CreateQuery(CountQuery);

let make = _children => {
  ...component,
  render: _self =>
    <FilmsQuery>
      ...(
           ({data}) =>
             <div>
               <h1> ("Star Wars 2: " |> ReasonReact.stringToElement) </h1>
               (
                 switch (data) {
                 | NoData
                 | Error(_) =>
                   ReasonReact.stringToElement("Something went wrong")
                 | Loading => ReasonReact.stringToElement("Loading")
                 | Data(result) =>
                   switch (result##allFilms) {
                   | None => "No Fils" |> ReasonReact.stringToElement
                   | Some(films) =>
                     "Films Found: "
                     ++ (films##totalCount |> int_from_opt |> string_of_int)
                     |> ReasonReact.stringToElement
                   }
                 }
               )
             </div>
         )
    </FilmsQuery>,
};

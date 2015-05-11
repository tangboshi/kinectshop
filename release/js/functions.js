// Qt-Funktionen
$(document).ready(function(){
    // Verlassen-Button
    $("#quit").click(Qt.quit);
    $("#login").click(function(){
        mySqlObj.testJs();
    });
});

// UI-Funktionen
$(document).ready(function() {
    // Nur ausgewählten Menüpunkt anzeigen
    $("#left-nav ul li[title]").click(function() {
      var x = $(this).attr("title");
      $(".active").toggleClass("active inactive");
      $("#" + x).toggleClass("active inactive");
    });
});

// Hilfsfunktionen

// Sortierfunktionen
$(document).ready(function(){
    // Verschiedene Datentypen miteinander vergleichen
    var compare = {
        name: function(a,b){
            if(a < b){
                return -1;
            }
            else{
                return a > b ? 1 : 0;
            }
        },
        number: function(a, b){
            return a-b;
        }
    }

    // Sortierfunktion
    $(".sortable").each(function(){
        var $table = $(this);
        var $tbody = $table.find("tbody");
        var $controls = $table.find("th");
        var rows = $tbody.find("tr").toArray();

        $controls.click(function(){
            var $header = $(this);
            var order = $header.data("sort");
            var column;

            if($header.is(".ascending")||$header.is(".descending")){
                $header.toggleClass("ascending descending");
                $tbody.append(rows.reverse());
            }
            else{
                $header.addClass("ascending");
                $header.siblings().removeClass("ascending descending");
                if(compare.hasOwnProperty(order)){
                    column = $controls.index(this);

                    rows.sort(function(a,b){
                      a = $(a).find("td").eq(column).text();
                      b = $(b).find("td").eq(column).text();
                      return compare[order](a,b);
                    });

                    $tbody.append(rows);
                }
            }
        });
    });
});

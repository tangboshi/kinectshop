// Click-Events
$(document).ready(function(){

    /* Noch zu erledigen:
     * connect() purchase() mit getBalance();
    */

    /* Cleaner Code:
     * js-Update von #acc-balance über signal balanceChanged()
     */

    // Verlassen-Button
    $("#quit").click(Qt.quit);

    // Login
    $("#login").click(function(){
        var username = $("#username").val();
        var password = $("#password").val();
        // Testfunktion
        // alert("Der eingegebene Username war: "+username+" Das eingegebene Passwort war: "+password);
        mySqlObj.login(username, password);
        if(mySqlObj.getLogin()){
           $("#login-form").toggleClass("active inactive");
           $("#logout-form").toggleClass("active inactive");
           $("#acc-username .accdd").html(mySqlObj.getUsername());
           $("#acc-balance .accdd").html(mySqlObj.getBalance());
           if(mySqlObj.getIsAdminLoggedIn()){
               $("#acc-isAdmin .accdd").html("Admin");
           }
           else{
               $("#acc-isAdmin .accdd").html("Kunde");
           }
        }
    });

    // Logout
    $("#logout").click(function(){
        mySqlObj.logout();
        if(!mySqlObj.getLogin()){
           $("#login-form").toggleClass("active inactive");
           $("#logout-form").toggleClass("active inactive");
           $("#acc-username .accdd").html(" --- ");
           $("#acc-balance .accdd").html(" --- ");
           $("#acc-isAdmin .accdd").html(" --- ");
        }
    });

    // User registrieren
    $("#register").click(function(){
        var username = $("#rUsername").val();
        var password = $("#rPassword").val();
        var repeatedPassword = $("#rPasswordRepeated").val();
        // Testfunktion
        // alert("Der eingegebene Username war: "+username+" Das eingegebene Passwort war: "+password+" Das wiederholte Passwort war: "+repeatedPassword);
        mySqlObj.registerUser(username, password, repeatedPassword);
    });

    // Guthaben aufladen
    $("#refillBalance").click(function(){
        var amount = $("#amount").val();
        // Testfunktion
        // alert("Der eingegebene Betrag war: "+amount+".");
        mySqlObj.refillBalance(amount);
        $("#acc-balance .accdd").html(mySqlObj.getBalance());
    });

    // Nur ausgewählten Menüpunkt anzeigen
    $("#left-nav ul li[title]").click(function(){
      var x = $(this).attr("title");
      $("#content .section.active").toggleClass("active inactive");
      $("#" + x).toggleClass("active inactive");
    });
});


//Javascript Insertion
// unter anderem listAllProducts() und showCart()
$(document).ready(function(){
    $("#shop").append(mySqlObj.listAllProducts());
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

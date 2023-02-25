<!DOCTYPE html>
<?php

include('database.php');
$contacts = get_contacts();
?>
<html>
    <head>
        <title>WST Assignment 4</title>
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <link rel="icon" href="./favicon-32x32.png" type="image/png" />
        <link rel="stylesheet" href="./css/bootstrap.min.css" />
        <link rel="stylesheet" href="./css/wst-assg4.css" />
    </head>

    <body>
        <div class="w-navbar">
            <div class="container">WST Assignment 4</div>
        </div>
        <div class="container w-content">
            <div class="row">
                <div class="col-md-4">
                     <?php

                        if( is_array($contacts)){
                    ?>
                        Data is there
                    <?php
                        }
                        else{
                             echo $contacts;  

                        }
                        

                     ?>
                </div>
            </div>
        </div>
    </body>
</html>

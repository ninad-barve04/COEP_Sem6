<!DOCTYPE html>
<?php

include('database.php');
$db = $conn;
$contacts = get_contacts($db);
?>
<html>
    <head>
        <title>WST Assignment 4 - Contact Manager</title>
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <link rel="stylesheet" href="./css/bootstrap.min.css" />
        <link rel="stylesheet" href="./css/wst-assg4.css" />
    </head>

    <body>
        <div class="w-navbar">
            <div class="container">
                <div class="btn btn-xs btn-light " style="float:left" class="m-2" ><a href="/index.php">Add Contact</a></div>
                <div class="btn btn-xs btn-light ml-2" style="float:left" class=" ml-2 m-2"><a href="/list.php">List Contacts</a></div>
                
                <div style="float:right" class="m-2">WST Assignment 4</div>
            </div>
        </div>
    
        <div class="container w-content">
            <div class="row">
                <div class="col-md-4">
                     <?php

                        if( is_array($contacts)){
                            foreach(  $contacts as $record){
                    ?>
                            <div class="card mb-3">
                                <div class="card-body">
                                    <div class="w-text1"><?php echo $record['firstname']??''; echo "  ".$record['lastname']??''; ?> </div>
                                    <div class="w-text2"><?php echo $record['username']??''; ?> </div>
                                    <div class="w-text3"><?php echo "Email: " . $record['email']??'' ; ?> </div>
                                    <div class="w-text3"><?php echo $record['state']??''; echo ", ".$record['country']??'' ; ?> </div>
                                </div>
                            </div>
                    <?php
                            }
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

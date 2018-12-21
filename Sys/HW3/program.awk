#!/bin/bash

$1~/^[a-z]{3}[0-9]{3}$/{
    # write into the array for each username
    userID[$1] = $1;

    # using the username as the index to ad the total login 
    userID_count[$1]++;

    #if not already exist in the array then write a new element
    #to find the last host since the file is backward with the reading process
    if (! last[$1])
        last[$1] = $3;

    #hour and min is for the newly read time
    hour=0;
    min=0;
#=====================================================================
    #if encounter 1st time and their time is finish
    if (userID_count[$1]==1)
    {    
        #if user have not log in from the system: calculate until "now"   
        if ($NF == "in")
        {
            #use hour and min to parse $7 time login
            #and calculate the total time up until "now"
            hour=int(substr($7,1,2));
            min=int(substr($7,4,2));
            thour[$1]=int(h)-int(hour);
            tmin[$1]=int(m)-int(min);
            #adjusting time if the minute become less than 0
            if (tmin[$1]<0)
            {
                thour[$1] = int(thour[$1])-1;
                tmin[$1] = int(tmin[$1]) + 60;
            }
        }
        #else just use the last column as the total time  for the first run
        else
        {
            total_time[$1] = $NF;
            thour[$1]=int(substr(total_time[$1],2,2));
            tmin[$1]=int(substr(total_time[$1],5,2));
        }
    }

    #add time for second encounter forward
    if (userID_count[$1] >= 2 )
    {   

        #parse info from the last column into hour and min
        hour=int(substr($NF,2,2));
        min=int(substr($NF,5,2));

        #adding to total time
        thour[$1]= int(thour[$1]) + int(hour);
        tmin[$1]= int(tmin[$1]) + int(min);

        #adjust time if the minute is bigger than 60    
        if (int(tmin[$1])>=60)
        {
            thour[$1] = int(thour[$1])+1;
            tmin[$1] = int(tmin[$1])-60;
        }
    }
}
END{
    for (i in userID_count)
    {
        print "User: "userID[i];
        print "         Last Host:      "last[i];
        printf"         Total Time:     %d:%.2d\n",int(thour[i]),int(tmin[i]);
        print "         Total Sessions: "userID_count[i];
    }
}

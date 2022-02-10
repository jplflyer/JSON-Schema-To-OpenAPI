#!/usr/bin/env bash
#
# Run Swagger-UI in a docker container with any files in the Examples directory.


URLS="[]"
for fname in Examples/*.json; do
    bname=`basename $fname`
    name="${bname%.*}"
    URLS=`echo $URLS | jq ". += [ { url: \"/Examples/$bname\", name: \"$name\" } ]"`
done

docker run --name swagger -d -p 9393:8080 -v $PWD/Examples:/usr/share/nginx/html/Examples -e URLS="${URLS}" swaggerapi/swagger-ui

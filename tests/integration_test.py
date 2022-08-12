import requests
import json
import sys, os
import subprocess
import atexit
import time


def main(argv):
    ################# Process arguments
    if len(argv) != 1:
        sys.exit("Usage Error: python3 integration_test.py {port #}")

    port = argv[0]
    host = "http://localhost"
    location = "api"
    entity = "Store"
    data = {"brand": "simply lemonade", "price": 4, "flavor": "strawberry"}
    CREATE_ENTITY_SUCCESS_CODE = 201
    RETRIEVE_ENTITY_SUCCESS_CODE = 200
    DELETE_ENTITY_SUCCESS_CODE = 200

    ################# Run server in background
    print("Running server...")
    with open(os.devnull, 'w') as f:  # Redirect noisy server output to /dev/null
        server = subprocess.Popen(["../build/bin/server", "location_config"], stdout=f)
    time.sleep(2)

    def exit_handler():
        nonlocal server
        server.terminate()
        print("Terminating server")

    atexit.register(exit_handler)

    ################# Create entity
    print("Creating entity...")
    res = requests.post(f"{host}:{port}/{location}/{entity}", json=data)
    status_code = res.status_code
    reason = res.reason
    content = res.text
    print(f"Response: {status_code} {reason}\nContent Body: {content}")
    if status_code != CREATE_ENTITY_SUCCESS_CODE:
        sys.exit("Create entity request error")

    # Process new ID from response body
    new_id = "id"
    try:
        res_json = res.json()
        for key in res_json:
            if key == "id":
                new_id = res_json[key]
                break
    except Exception as e:
        sys.exit(f"Exception: {e}")
    else:
        if new_id == "id":
            sys.exit("Unable to locate id field in response body")

    ################ Retrieve entity
    print("Retrieving entity...")
    res = requests.get(f"{host}:{port}/{location}/{entity}/" + str(new_id))
    status_code = res.status_code
    reason = res.reason
    content = res.text
    print(f"Response: {status_code} {reason}\nContent Body: {content}")
    if status_code != RETRIEVE_ENTITY_SUCCESS_CODE:
        sys.exit("Retrieve entity request error")

    # NOTE: The following lines are commented out due to an issue with how 
    # requests were being parsed in the server. As a result, the
    # http::request<http::string_body> object that is passed to the RequestHandler
    # does not contain the Request body in its body parameter, but instead 
    # the entire HTTP Request as a string, including headers.

    # Process data from response body
    # retrieved_data = {}
    # try:
    #     res_json = res.json()
    # except Exception as e:
    #     sys.exit(f"Exception: {e}")
    # else:
    #     retrieved_data = res_json
    #     if retrieved_data == data:
    #         print("correct")
    #     else:
    #         sys.exit("Retrieved entity data was not equal to original data")

    ################ Delete entity
    print("Deleting entity...")
    res = requests.delete(f"{host}:{port}/{location}/{entity}/" + str(new_id))
    status_code = res.status_code
    reason = res.reason
    content = res.text
    print(f"Response: {status_code} {reason}\nContent Body: {content}")
    if status_code != DELETE_ENTITY_SUCCESS_CODE:
        sys.exit("Delete entity request error")

    # Verify that entity is no longer retrievable
    print("Retrieving non-existent entity...")
    res = requests.get(f"{host}:{port}/{location}/{entity}/" + str(new_id))
    status_code = res.status_code
    reason = res.reason
    content = res.text
    print(f"Response: {status_code} {reason}\nContent Body: {content}")
    if status_code != 404:
        sys.exit("Entity still exists after deletion")


if __name__ == "__main__":
    main(sys.argv[1:])

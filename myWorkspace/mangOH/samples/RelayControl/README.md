# mangOH/RelayControl

A simple RESTful app to control relay boards.

## Build

To build the system, you can do:
```bash
cp relayControl.sdef ../../mangoh.sdef
cd ../../../legato
make wp85
```

## Support

* mangOH

## REST API

| Method | Request                       | Action                           |
|--------|-------------------------------|----------------------------------|
| GET    | /                             | Help: list all routes            |
| GET    | /relays                       | List all relays and their states |
| GET    | /relays/\<relay_id\>          | Get state for one relay          |
| POST   | /relays/\<relay_id\>          | Set state for one relay          |

## Sample usage

### List all relays

```bash
curl http://10.1.28.238:8080/relays | jq -r .
```
```json
[
  {
    "state": 0,
    "id": "1"
  },
  {
    "state": 1,
    "id": "0"
  },
  {
    "state": 1,
    "id": "3"
  },
  {
    "state": 0,
    "id": "2"
  },
  {
    "state": 1,
    "id": "5"
  },
  {
    "state": 1,
    "id": "4"
  },
  {
    "state": 1,
    "id": "7"
  },
  {
    "state": 1,
    "id": "6"
  }
]
```

### Get state of one relay

```bash
curl http://10.1.28.238:8080/relays/3 | jq -r .
```
```json
{
  "state": 1,
  "id": "3"
}
```

### Set state for one relay

```bash
curl -X POST --data "state=1" http://10.1.28.238:8080/relays/3 | jq -r .
```
```json
{
  "state": 1,
  "id": "3"
}
```


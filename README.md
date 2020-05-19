# ele2-coffee-machine

Coffee machine IoT project. Easily check the amount of coffee in the pan via a Telegram bot. Electronics II course project.

## Table of Contents

* [Directories](#directories)
* [Architecture](#architecture)
* [Setting up the project](#setting-up-the-project)
    + [1. Install Docker](#1-install-docker)
    + [2. Clone or download the repository](#2-clone-or-download-the-repository)
    + [3. Set up the database and Node-RED](#3-set-up-the-database-and-node-red)
        - [3.1 PostgreSQL database](#31-postgresql-database)
        - [3.2 Node-RED](#32-node-red)
            * [Starting](#starting)
            * [Importing flows](#importing-flows)
            * [Configure database connection](#configure-database-connection)
* [Sensor schematic](#sensor-schematic)

## Directories

| Directory         | Description                                                                                            |
|-------------------|--------------------------------------------------------------------------------------------------------|
| bot               | Telegram bot which serves the coffee status from the database                                          |
| coffee-sensor     | The code for the ESP8266 sensor                                                                        |
| kicad             | Electronic wiring schema for the sensor                                                                |
| ota-update-server | Coffee sensor over-the-air firmware update server                                                      |
| serial-dumper     | Utility for dumping numbers printed to serial from the coffee-sensor. Useful for generating test data. |

## Architecture

The project contains multiple components:
- The sensor attached to the coffee machine (`coffee-sensor`)
- An OTA firmware update server for the sensor (`ota-update-server`)
- A Telegram bot (`bot`)
- A [Node-RED](https://nodered.org/) instance responsible for receiving the data from the sensor and inserting it into the database
- A [PostgreSQL](https://www.postgresql.org/) database which contains the data sent by the sensor

![](https://github.com/cxcorp/ele2-coffee-machine/raw/master/documentation/architecture-v2.png)

## Setting up the project

### 1. Install Docker

The project makes heavy use of Docker to make development easier, to make moving from development to production easier, and to allow a wider range of execution environments (self-hosted, cloud PaaS, cloud IaaS). The `docker-compose.yml` file describes all of our services which will be started.

From [What is a container?](https://www.docker.com/resources/what-container):

> A container is a standard unit of software that packages up code and all its dependencies so the application runs quickly and reliably from one computing environment to another. A Docker container image is a lightweight, standalone, executable package of software that includes everything needed to run an application: code, runtime, system tools, system libraries and settings.

To get started, [install Docker and Docker-Compose](https://devopswithdocker.com/part0/#installing-docker).

If you're on Linux, consider following the [Post-installation steps for Linux](https://docs.docker.com/engine/install/linux-postinstall/) to add yourself to the `docker` group so that you don't need to `sudo` all of the docker commands.

### 2. Clone or download the repository

Click on the big green button at the top right of the Github repository. The following steps may refer to certain files (e.g. `docker-compose.yml`). These files are found from the cloned or downloaded directory.

### 3. Set up the database and Node-RED

![](https://github.com/cxcorp/ele2-coffee-machine/blob/master/documentation/install-instructions-images/arch-install-step-nodered.png?raw=true)

As seen from the diagram, the sensor sends its data to the database via the Node-RED instance. Let's first set this part up.

From [https://nodered.org/](https://nodered.org/):

> Node-RED is a programming tool for wiring together hardware devices, APIs and online services in new and interesting ways.
>
> It provides a browser-based editor that makes it easy to wire together flows using the wide range of nodes in the palette that can be deployed to its runtime in a single-click.

#### 3.1 PostgreSQL database

*If you are using a PostgreSQL database hosted elsewhere, you can skip this step. You may be interested in hosted PostgreSQL services, such as [ElephantSQL](https://www.elephantsql.com/) which has a fair free-tier.*

Take a look at the [definition of the `db` service](https://github.com/cxcorp/ele2-coffee-machine/blob/76839ff80d9062c5a7eeb944c4840317e7a75f39/docker-compose.yml#L51-L61) in the `docker-compose.yml` file.

The key parts here to change are:

```sh
POSTGRES_USER=coffee       # superuser username
POSTGRES_PASSWORD=1234     # superuser password
POSTGRES_DB=coffee         # database name
```

When bringing up the postgres container for the first time, it will read these values to create a superuser and a database. 

**Edit the file and change the passwords to something secure, and store them somewhere safe.**

**Production notice:** If running in production, create a new postgres user specifically for the `coffee` db, and don't use the superuser.

After this is done, you can start the database by doing
```
> docker-compose up -d db
```

Here the `up` command instructs docker-compose to find a service from the `docker-compose.yml` file named `db`, and to start it in the background (`-d`) so that we don't need to keep the terminal open.

After running this command, the database will create the user and the database, and then restart. You can view its progress by doing `docker-compose logs -f db`, and exit the logs view with `CTRL-C`.

#### 3.2 Node-RED

##### Starting

After bringing up the dabase, start nodered with
```
> docker-compose up -d nodered
```
You will see a new directory named `nodered-data/` appear. This directory is a [volume mount](https://devopswithdocker.com/part1/#volumes-bind-mount), meaning that it connects a directory from your computer to the directory inside the container.

Open up your browser and go to [http://localhost:1880](http://localhost:1880). You may need to wait a second for nodered to start.

##### Importing flows

To configure nodered to do what we want, follow the [instructions here to import](https://nodered.org/docs/user-guide/editor/workspace/import-export) the `nodered-flow.json` file.

![](https://raw.githubusercontent.com/cxcorp/ele2-coffee-machine/master/documentation/install-instructions-images/nodered-import.png)

##### Configure database connection

After importing the flow, you should see a cyan `postgres` node in the graph. Double-click on this node. A sidebar should appear. Click on the Edit button of the Server row:

![](https://raw.githubusercontent.com/cxcorp/ele2-coffee-machine/master/documentation/install-instructions-images/nodered-configure-psql.png)

Insert your database connection details here. Refer to the details configured for the PostgreSQL database previously. Again, if you're using a hosted PostgreSQL instance from elsewhere, insert its details here instead.

- **Host**: `db` (or if you've renamed the `db` service, its name)
- **Port**: `5432`
- **Database**: Same as `POSTGRES_DB`
- **Username**: Same as `POSTGRES_USER`
- **Password**: Same as `POSTGRES_PASSWORD`

Click on Add, then select the newly created Server from the drop-down. Click Done.

Click the red Deploy button in the top right. This will apply the changes we've created. We now have the websocket endpoint available for our sensor at `ws://localhost:1880/ws/weight`. Remember this URL. The `/ws/weight` part is configurable in nodered.


* docker & docker-compose:
- surrounding infrastructure
    - bot
        - botfather token
        - db migrations
    - update server
        - self-signed certificate
        - google cloud service account
            - json key
            - drive api
            - sheets api
            - drive folder and sheets structure
                - named range
                - semver
    * nodered
        * import flow
        * url
    * postgres
        * credentials
- sensor
    - public.key, private.key
    - versioning: semver
    - URLs
    - scale calibration
    - tare

## Sensor schematic

![](https://github.com/cxcorp/ele2-coffee-machine/raw/master/documentation/schematic-v2.png)

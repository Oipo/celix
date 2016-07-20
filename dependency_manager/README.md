Title: Apache Celix Dependency Manager

## Introduction

The Dependency Manager contains a static library which can be iuse to manage (dynamic) services on a higher abstraction level in a declarive style. 
The Apache Celix Dependency Manager is inspired by the [Apache Felix Dependency Manager](http://felix.apache.org/documentation/subprojects/apache-felix-dependency-manager.html).

## Components

Components are the main building blocks for OSGi applications. They can publish services, and/or they can have dependencies. These dependencies will influence their life cycle as component will only be activated when all required dependencies are available.

Within Apache Celix a component is expected to have a set of functions where the first argument is a handle to the component (e.g. self/this). How this is achieved is up the the user, for some examples how this can be done see the example in the Apache Celix Project. 

The Dependency Manager, as part of a bundle, shares the generic bundle life cycle explained in the OSGi specification. 
Each component you define gets its own life cycle. The component life cycle is depicted in the state diagram below.

![Component Life Cycle](doc-images/statediagram.png)

Changes in the state of the component will trigger the following life cycle callback functions:

    `init`,
    `start`,
    `stop` and
    `deinit`.

The callback functions can be specified by using the component\_setCallbacks.

## DM Parts

The Dependency Manager consist out of four main parts: `DM (Dependency Manager) Activator`, `Dependency Manager`, `DM Component` and `DM Service Dependency`.

### DM Activator

The `DM Activator implements a "normal" Celix bundle activator and depends on four functions which needs to be implemented by the user of the Depedency Manager:
 - `dm\_create` : Should be used to allocated and initialize a dm activator structure. If needed this structure can be used to store object during the lifecycle of the bundle.
 - `dm\_init` : Should be used to interact with the `Dependency Manager`. Here a user can components, service dependencies and provided services. 
 - `dm\_destroy` : Should be used to deinitialize and deallocate objects created in the `dm\_create` function.


### Dependency Manager

The `Dependency Manager` act as an entry point to add or remove DM Components. The `Dependency Manager` is provided to the `dm\_init` functoin.

### DM Component

The `DM Component` manages the life cycle of the component. For example, when all required service dependencies are aviable the `DM Component` will call the `start` specified callback function of the component. 

The `component\_setImplementation` function can be used to specify which component handle to use. 
The `component\_addInterface` can be used to specify one additional service provided by the component. 
The `component\_addServiceDependency` can be used to specify one additional service dependency.

### Dm Service Dependency 

The `DM Service Dependency` can be used to specify service dependencies for a component. i

When these dependencies are set to required the `DM Component` will ensure that components will only be started when all quired dependencies are aviable and stop the component if any of the required dependencies are removed. This feature should prevent a lot of boiler plating code compared to usign a service tracker or services references directly. 

A service dependency update strategy can also be specified. Default this strategy is set to `DM\_SERVICE\_DEPENDENCY\_STRATEGY\_SUSPEND` this stragegy will stop and start (suspend) a component when any of the specified service dependencies change (are removed, added or modified). When correclty used this strategy removes the need for locking services during updates/invocation. See the dependency manager example for more details.

The `serviceDependency\_setCallbacks` function can be used to specify the function callback used when services are added, set, removed or modified. 
The `serviceDependency\_setRequired` function can be used to specify if a service dependency is required.
The `serviceDependency\_setStrategy` function can be used to specify a service dependency update strategy (suspend or locking).

### Snippets

### DM Bundle Activator

The next snippet shows a dm bundle activator and how to add components to the dependency manager.
```C

//exmpl_activator.c
#include <dm\_activator.h>
#include <stdlib.h>

struct dm_exmpl_activator {
    exmpl_t* exmpl;
};

celix_status_t dm_create(bundle_context_pt context, void **userData) {                                                                                                                                             
    *userData = calloc(1, sizeof(struct dm_exmpl_activator));
    return *userData != NULL ? CELIX_SUCCESS : CELIX_ENOMEM;
}

celix_status_t dm_init(void * userData, bundle_context_pt context, dm_dependency_manager_pt manager) {
    celix_status_t status = CELIX_SUCCESS;
    struct dm_exmpl_activator *act = (struct dm_exmpl_activator*)userData;

    act->exmpl = exmpl_create();
    if (act->exmpl != NULL) {
        dm_component_pt cmp;
        component_create(context, "Example Component", &cmp);
        component_setImplementation(cmp, act->exmpl);

        dependencyManager_add(manager, cmp);
    } else {
        status = CELIX_ENOMEM;
    }

    return status;
}

celix_status_t dm_destroy(void * userData, bundle_context_pt context, dm_dependency_manager_pt manager) {
    celix_status_t status = CELIX_SUCCESS;
    struct dm_exmpl_activator *act = (struct dm_exmpl_activator*)userData;

    if (act->exmpl != NULL) {
        exmpl_destroy(act->exmpl);
    }
    free(act);

    return CELIX_SUCCESS;
}  
```

TODO add cmp callbacks
TODO add interface
TODO add service dependency and callbacks

### Example

For a more complete overview what can be done with the dependency manager please look at the dm_example example. Should be located at 'examples/dm_example'.
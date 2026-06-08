// Created by PC on 2/06/2026.
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <stdexcept>
using namespace std;

struct Producto {
    int codigo;
    char nombre[40];
    char categoria[30];
    int stock;
    float precio;
    bool activo;
    int ventasAcumuladas;
};
struct Venta {
    int idVenta;
    char fecha[15];
    float subtotal;
    float IVA;
    float descuento;
    float total;
    int cantidadArticulos;
};
struct DetalleVenta {
    int idVenta;
    int codigoProducto;
    int cantidad;
    float precioUnitario;
    float subtotal;
};
// ================== PROTOTIPOS ==================
void menuPrincipal();
void menuProductos();
void menuVentas();
void menuReportes();
void menuUtilidades();
// ====== PROTOTIPOS MODULO 1 - PRODUCTOS ======
void registrarProducto();
void listarProductos();
void buscarPorCodigo(int codigoBuscado);
void buscarPorNombre(const char* nombreBuscado);
void actualizarStock(int codigoBuscado, int nuevoStock);
void modificarPrecio(int codigoBuscado, float nuevoPrecio);
void desactivarProducto(int codigoBuscado);
// ====== PROTOTIPOS MODULO 2 - VENTAS ======
void crearVenta();
float calcularIVA(float subtotal);
float calcularDescuento(float subtotal, int cantidadTotal);
void guardarVenta(const Venta &v);
void guardarDetalle(const DetalleVenta &d);
void descontarStock(int codigoBuscado, int cantidadVendida);

// ====== PROTOTIPOS MODULO 3 - REPORTES ======
void reporteMenorStock();
void reporteMasVendido();
void ordenarPorPrecio();
void reporteVentasPorMes();
void menuReportes();

// ====== PROTOTIPOS MODULO 4 - UTILIDADES ======
void exportarReporteTXT();
void reiniciarInventario();
void estadisticasGenerales();
void menuUtilidades();

int main() {
    cout << "=================================\n";
    cout << "SISTEMA DE VENTAS E INVENTARIO\n";
    cout << "Luis Fernando Cruz Franco\n";
    cout << "=================================\n";
    menuPrincipal();
    cout << "\nPrograma finalizado.\n";
    cout << "Desarrollado por: Luis Fernando Cruz Franco\n";
    cout << "Estudiante de Ingenieria en Sistemas.\n";
    return 0;
}

void menuPrincipal() {
    int opcion;
    do {
        cout << "\n===== MENU PRINCIPAL =====" << endl;
        cout << "1. Gestion de productos" << endl;
        cout << "2. Ventas" << endl;
        cout << "3. Reportes" << endl;
        cout << "4. Utilidades" << endl;
        cout << "0. Salir" << endl;
        cout << "Elija una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: menuProductos();
                break;
            case 2: menuVentas();
                break;
            case 3: menuReportes();
                break;
            case 4: menuUtilidades();
                break;
            case 0: cout << "Saliendo del programa...\n";
                break;
            default: cout << "Opcion invalida!\n";
        }
    } while (opcion != 0);
}

// ================== MENÚ PRODUCTOS ==================
void menuProductos() {
    int opcion;
    do {
        cout << "\n===== MENU GESTION DE PRODUCTOS =====" << endl;
        cout << "1. Registrar producto" << endl;
        cout << "2. Listar productos" << endl;
        cout << "3. Buscar por codigo" << endl;
        cout << "4. Buscar por nombre" << endl;
        cout << "5. Actualizar stock" << endl;
        cout << "6. Modificar precio" << endl;
        cout << "7. Desactivar producto" << endl;
        cout << "0. Regresar" << endl;
        cout << "Elija una opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1: registrarProducto(); break;
            case 2: listarProductos(); break;
            case 3: {
                int codigo;
                cout << "Codigo: "; cin >> codigo;
                buscarPorCodigo(codigo);
                break;
            }
            case 4: {
                char nombre[40];
                cout << "Nombre: ";
                cin.ignore();
                cin.getline(nombre, 40);
                buscarPorNombre(nombre);
                break;
            }
            case 5: {
                int codigo, nuevoStock;
                cout << "Codigo: "; cin >> codigo;
                cout << "Nuevo stock: "; cin >> nuevoStock;
                actualizarStock(codigo, nuevoStock);
                break;
            }
            case 6: {
                int codigo; float nuevoPrecio;
                cout << "Codigo: "; cin >> codigo;
                cout << "Nuevo precio: "; cin >> nuevoPrecio;
                modificarPrecio(codigo, nuevoPrecio);
                break;
            }
            case 7: {
                int codigo;
                cout << "Codigo: "; cin >> codigo;
                desactivarProducto(codigo);
                break;
            }
            case 0: cout << "Regresando al menu principal...\n"; break;
            default: cout << "Opcion invalida!\n";
        }
    } while(opcion != 0);
}

// ================== FUNCIONES PRODUCTOS ==================
void registrarProducto() {
    Producto producto;
    ofstream archivo("productos.dat", ios::binary | ios::app);
    if(!archivo) {
        cout << "Error al abrir archivo.\n";
        return;
    }

    try {
        cout << "\n===== REGISTRO PRODUCTO =====\n";
        cout << "Codigo: ";
        cin >> producto.codigo;
        if(cin.fail()) throw runtime_error("Entrada inválida en código");

        cin.ignore();
        cout << "Nombre: ";
        cin.getline(producto.nombre, 40);

        cout << "Categoria: ";
        cin.getline(producto.categoria, 30);

        cout << "Stock: ";
        cin >> producto.stock;
        if(cin.fail()) throw runtime_error("Entrada invalida en stock");

        cout << "Precio: ";
        cin >> producto.precio;
        if(cin.fail()) throw runtime_error("Entrada invalida en precio");

        producto.activo = true;
        producto.ventasAcumuladas = 0;

        archivo.write((char*)&producto, sizeof(producto));
        cout << "Producto registrado correctamente.\n";
    }
    catch(const exception &e) {
        cout << "Error: " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }

    archivo.close();
}

void listarProductos() {
    try {
        ifstream archivo("productos.dat", ios::binary);
        if(!archivo) throw runtime_error("No existe informacion almacenada.");

        Producto p;
        cout << "\n===== LISTADO DE PRODUCTOS =====\n";
        while(archivo.read((char*)&p, sizeof(p))) {
            cout << "\nCodigo: " << p.codigo;
            cout << "\nNombre: " << p.nombre;
            cout << "\nCategoria: " << p.categoria;
            cout << "\nStock: " << p.stock;
            cout << "\nPrecio: Q" << p.precio;
            cout << "\nActivo: " << (p.activo ? "Si" : "No") << endl;
        }
        archivo.close();
    }
    catch(const exception &e) {
        cout << "Error: " << e.what() << endl;
    }
}

void buscarPorCodigo(int codigoBuscado) {
    ifstream archivo("productos.dat", ios::binary);
    if(!archivo) { cout << "Error al abrir archivo.\n"; return; }
    Producto p; bool encontrado = false;
    while(archivo.read((char*)&p, sizeof(p))) {
        if(p.codigo == codigoBuscado) {
            cout << "Producto encontrado: " << p.nombre << " (Stock: " << p.stock << ")";
            encontrado = true; break;
        }
    }
    if(!encontrado) cout << "No existe producto con ese codigo.\n";
    archivo.close();
}

void buscarPorNombre(const char* nombreBuscado) {
    ifstream archivo("productos.dat", ios::binary);
    if(!archivo) { cout << "Error al abrir archivo.\n"; return; }
    Producto p; bool encontrado = false;
    while(archivo.read((char*)&p, sizeof(p))) {
        if(strcmp(p.nombre, nombreBuscado) == 0) {
            cout << "Producto encontrado: " << p.codigo << " (Stock: " << p.stock << ")";
            encontrado = true; break;
        }
    }
    if(!encontrado) cout << "No existe producto con ese nombre.\n";
    archivo.close();
}

void actualizarStock(int codigoBuscado, int nuevoStock) {
    fstream archivo("productos.dat", ios::binary | ios::in | ios::out);
    if(!archivo) { cout << "Error al abrir archivo.\n"; return; }
    Producto p; bool encontrado = false;
    while(archivo.read((char*)&p, sizeof(p))) {
        if(p.codigo == codigoBuscado) {
            p.stock = nuevoStock;
            archivo.seekp(-sizeof(p), ios::cur);
            archivo.write((char*)&p, sizeof(p));
            cout << "Stock actualizado.\n";
            encontrado = true; break;
        }
    }
    if(!encontrado) cout << "Producto no encontrado.\n";
    archivo.close();
}

void modificarPrecio(int codigoBuscado, float nuevoPrecio) {
    fstream archivo("productos.dat", ios::binary | ios::in | ios::out);
    if(!archivo) { cout << "Error al abrir archivo.\n"; return; }
    Producto p; bool encontrado = false;
    while(archivo.read((char*)&p, sizeof(p))) {
        if(p.codigo == codigoBuscado) {
            p.precio = nuevoPrecio;
            archivo.seekp(-sizeof(p), ios::cur);
            archivo.write((char*)&p, sizeof(p));
            cout << "Precio modificado.\n";
            encontrado = true; break;
        }
    }
    if(!encontrado) cout << "Producto no encontrado.\n";
    archivo.close();
}

void desactivarProducto(int codigoBuscado) {
    fstream archivo("productos.dat", ios::binary | ios::in | ios::out);
    if(!archivo) { cout << "Error al abrir archivo.\n"; return; }
    Producto p; bool encontrado = false;
    while(archivo.read((char*)&p, sizeof(p))) {
        if(p.codigo == codigoBuscado) {
            p.activo = false;
                        archivo.seekp(-sizeof(p), ios::cur);
            archivo.write((char*)&p, sizeof(p));
            cout << "\nProducto desactivado.\n";
            encontrado = true;
            break;
        }
    }
    if(!encontrado) cout << "\nProducto no encontrado.\n";
    archivo.close();
}

// ================== FUNCIONES DE VENTAS ==================
float calcularIVA(float subtotal) {
    return subtotal * 0.12;
}

float calcularDescuento(float subtotal, int cantidadTotal) {
    float descuento = 0;
    if (subtotal > 500) {
        descuento += subtotal * 0.05;
    }
    if (cantidadTotal % 5 == 0) {
        descuento += 25;
    }
    return descuento;
}

void guardarVenta(const Venta &v) {
    ofstream archivoVentas("ventas.dat", ios::binary | ios::app);
    if(!archivoVentas) {
        cout << "\nError! no se pudo abrir el archivo ventas.dat\n";
        return;
    }
    archivoVentas.write((char*)&v, sizeof(Venta));
    archivoVentas.close();
    cout << "\nVenta guardada correctamente.\n";
}

void guardarDetalle(const DetalleVenta &d) {
    ofstream archivoDetalles("detalles.dat", ios::binary | ios::app);
    if(!archivoDetalles) {
        cout << "\nError! no se pudo abrir el archivo detalles.dat\n";
        return;
    }
    archivoDetalles.write((char*)&d, sizeof(DetalleVenta));
    archivoDetalles.close();
}

void descontarStock(int codigoBuscado, int cantidadVendida) {
    fstream archivo("productos.dat", ios::binary | ios::in | ios::out);
    if(!archivo) {
        cout << "\nError! no se pudo abrir productos.dat\n";
        return;
    }

    Producto p;
    bool encontrado = false;

    while(archivo.read((char*)&p, sizeof(p))) {
        if(p.codigo == codigoBuscado && p.activo) {
            if(cantidadVendida <= p.stock) {
                p.stock -= cantidadVendida;
                p.ventasAcumuladas += cantidadVendida;

                // retroceder el puntero para sobrescribir el registro
                archivo.seekp(-(std::streamoff)sizeof(p), ios::cur);
                archivo.write((char*)&p, sizeof(p));

                cout << "\nStock actualizado para " << p.nombre << endl;
            } else {
                cout << "\nStock insuficiente para " << p.nombre << endl;
            }
            encontrado = true;
            break;
        }
    }

    if(!encontrado) cout << "\nProducto no encontrado.\n";
    archivo.close();
}


// ================== CREAR VENTA ==================
void crearVenta() {
    Venta v;

    cout << "\nIngrese el ID de venta: ";
    cin >> v.idVenta;
    cout << "Ingrese la fecha (dd/mm/yyyy): ";
    cin >> v.fecha;

    v.subtotal = 0;
    v.IVA = 0;
    v.descuento = 0;
    v.total = 0;
    v.cantidadArticulos = 0;

    int seguir = 1;
    while(seguir == 1) {
        int codigo, cantidad;
        cout << "\nIngrese el codigo de producto: ";
        cin >> codigo;
        cout << "Cantidad: ";
        cin >> cantidad;

        fstream archivo("productos.dat", ios::binary | ios::in | ios::out);
        if(!archivo) {
            cout << "\nError! no se pudo abrir productos.dat\n";
            return;
        }
        Producto p;
        bool encontrado = false;
        while(archivo.read((char*)&p, sizeof(p))) {
            if(p.codigo == codigo && p.activo) {
                encontrado = true;
                if (cantidad <= p.stock) {
                    v.subtotal += p.precio * cantidad;
                    v.cantidadArticulos += cantidad;

                    DetalleVenta d;
                    d.idVenta = v.idVenta;
                    d.codigoProducto = codigo;
                    d.cantidad = cantidad;
                    d.precioUnitario = p.precio;
                    d.subtotal = p.precio * cantidad;
                    guardarDetalle(d);

                    // ✅ Actualizar stock y ventas acumuladas
                    p.stock -= cantidad;
                    p.ventasAcumuladas += cantidad;

                    // Reescribir producto actualizado en el archivo
                    archivo.seekp(-sizeof(p), ios::cur);
                    archivo.write((char*)&p, sizeof(p));

                    cout << "\nProducto agregado y stock actualizado: " << p.nombre << endl;
                } else {
                    cout << "Stock insuficiente.\n";
                }
                break;
            }
        }
        archivo.close();
        if(!encontrado) cout << "\nProducto no encontrado.\n";

        cout << "Desea agregar otro producto (1=Si, 0=No): ";
        cin >> seguir;
    }

    v.IVA = calcularIVA(v.subtotal);
    v.descuento = calcularDescuento(v.subtotal, v.cantidadArticulos);
    v.total = v.subtotal + v.IVA - v.descuento;

    cout << fixed << setprecision(2);
    cout << "\n===== RESUMEN DE VENTA =====" << endl;
    cout << "ID Venta: " << v.idVenta << endl;
    cout << "Fecha: " << v.fecha << endl;
    cout << "Subtotal: Q" << v.subtotal << endl;
    cout << "IVA: Q" << v.IVA << endl;
    cout << "Descuento: Q" << v.descuento << endl;
    cout << "Total: Q" << v.total << endl;

    guardarVenta(v);
}

// ================== MENÚ VENTAS ==================
void menuVentas() {
    int opcion;
    do {
        cout << "\n===== MENU VENTAS =====" << endl;
        cout << "1. Crear venta" << endl;
        cout << "0. Regresar" << endl;
        cout << "Elija una opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1: crearVenta(); break;
            case 0: cout << "Regresando al menu principal...\n"; break;
            default: cout << "Opcion invalida!\n";
        }
    } while(opcion != 0);
}

// ================== FUNCIONES DE REPORTES ==================
void reporteMenorStock() {
    ifstream archivo("productos.dat", ios::binary);
    if(!archivo) { cout << "Error al abrir productos.dat\n"; return; }
    Producto p;
    int menor = INT_MAX;
    Producto menorProd;
    while(archivo.read((char*)&p, sizeof(p))) {
        if(p.activo && p.stock < menor) {
            menor = p.stock;
            menorProd = p;
        }
    }
    archivo.close();
    if(menor != INT_MAX)
        cout << "\nProducto con menor stock: " << menorProd.nombre
             << " (Stock: " << menorProd.stock << ")\n";
    else
        cout << "\nNo hay productos registrados.\n";
}

void reporteMasVendido() {
    ifstream archivo("productos.dat", ios::binary);
    if(!archivo) { cout << "Error al abrir productos.dat\n"; return; }
    Producto p;
    int mayor = -1;
    Producto masVend;
    while(archivo.read((char*)&p, sizeof(p))) {
        if(p.activo && p.ventasAcumuladas > mayor) {
            mayor = p.ventasAcumuladas;
            masVend = p;
        }
    }
    archivo.close();
    if(mayor != -1)
        cout << "\nProducto mas vendido: " << masVend.nombre
             << " (Ventas: " << masVend.ventasAcumuladas << ")\n";
    else
        cout << "\nNo hay productos registrados.\n";
}

// Ordenar productos por precio usando Bubble Sort
void ordenarPorPrecio() {
    ifstream archivo("productos.dat", ios::binary);
    if(!archivo) { cout << "Error al abrir productos.dat\n"; return; }
    vector<Producto> lista;
    Producto p;
    while(archivo.read((char*)&p, sizeof(p))) lista.push_back(p);
    archivo.close();

    for(size_t i=0; i<lista.size()-1; i++) {
        for(size_t j=0; j<lista.size()-i-1; j++) {
            if(lista[j].precio > lista[j+1].precio) {
                swap(lista[j], lista[j+1]);
            }
        }
    }

    cout << "\nProductos ordenados por precio:\n";
    for(auto &prod : lista) {
        cout << prod.nombre << " - Q" << prod.precio << endl;
    }
}

void reporteVentasPorMes() {
    float ventasPorMes[12] = {0};

    ifstream archivo("ventas.dat", ios::binary);
    if(!archivo) {
        cout << "Error al abrir ventas.dat\n";
        return;
    }
    Venta v;
    while(archivo.read((char*)&v, sizeof(Venta))) {
        int dia, mes, anio;
        if(sscanf(v.fecha, "%d/%d/%d", &dia, &mes, &anio) == 3) {
            if(mes >= 1 && mes <= 12) {
                ventasPorMes[mes-1] += v.total;
            }
        }
    }
    archivo.close();
    cout << "\n===== VENTAS POR MES =====\n";
    for(int i=0; i<12; i++) {
        cout << "Mes " << (i+1) << ": Q" << fixed << setprecision(2) << ventasPorMes[i] << endl;
    }
}
// ================== MENÚ REPORTES ==================
void menuReportes() {
    int opcion;
    do {
        cout << "\n===== MENU REPORTES =====" << endl;
        cout << "1. Producto con menor stock" << endl;
        cout << "2. Producto mas vendido" << endl;
        cout << "3. Ordenar productos por precio" << endl;
        cout << "4. Ventas por mes" << endl;
        cout << "0. Regresar" << endl;
        cout << "Elija una opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1: reporteMenorStock(); break;
            case 2: reporteMasVendido(); break;
            case 3: ordenarPorPrecio(); break;
            case 4: reporteVentasPorMes(); break;
            case 0: cout << "Regresando al menu principal...\n"; break;
            default: cout << "Opcion invalida!\n";
        }
    } while(opcion != 0);
}

// ================== FUNCIONES DE UTILIDADES ==================
void exportarReporteTXT() {
    ofstream txt("reporte.txt");
    if(!txt) { cout << "Error al crear reporte.txt\n"; return; }
    txt << "=== Reporte General ===\n";
    txt << "Este archivo puede incluir estadísticas de productos y ventas.\n";
    txt.close();
    cout << "Reporte exportado a reporte.txt\n";
}

void reiniciarInventario() {
    cout << "¿Seguro que desea reiniciar inventario? (1=Si, 0=No): ";
    int confirm; cin >> confirm;
    if(confirm == 1) {
        fstream archivo("productos.dat", ios::binary | ios::in | ios::out);
        Producto p;
        while(archivo.read((char*)&p, sizeof(p))) {
            p.stock = 0;
            archivo.seekp(-sizeof(p), ios::cur);
            archivo.write((char*)&p, sizeof(p));
        }
        archivo.close();
        cout << "Inventario reiniciado.\n";
    }
}

void estadisticasGenerales() {
    ifstream archivo("productos.dat", ios::binary);
    if(!archivo) { cout << "Error al abrir productos.dat\n"; return; }
    Producto p;
    int totalProductos = 0, stockTotal = 0;
    float sumaPrecios = 0;
    while(archivo.read((char*)&p, sizeof(p))) {
        if(p.activo) {
            totalProductos++;
            stockTotal += p.stock;
            sumaPrecios += p.precio;
        }
    }
    archivo.close();
    if(totalProductos > 0) {
        cout << "\n=== Estadisticas Generales ===\n";
        cout << "Productos activos: " << totalProductos << endl;
        cout << "Stock total: " << stockTotal << endl;
        cout << "Precio promedio: Q" << (sumaPrecios/totalProductos) << endl;
    } else {
        cout << "\nNo hay productos activos.\n";
    }
}

// ================== MENÚ UTILIDADES ==================
void menuUtilidades() {
    int opcion;
    do {
        cout << "\n===== MENU UTILIDADES =====" << endl;
        cout << "1. Exportar reporte a TXT" << endl;
        cout << "2. Reiniciar inventario" << endl;
        cout << "3. Mostrar estadísticas generales" << endl;
        cout << "0. Regresar" << endl;
        cout << "Elija una opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1: exportarReporteTXT(); break;
            case 2: reiniciarInventario(); break;
            case 3: estadisticasGenerales(); break;
            case 0: cout << "Regresando al menu principal...\n"; break;
            default: cout << "Opcion invalida!\n";
        }
    } while(opcion != 0);
}



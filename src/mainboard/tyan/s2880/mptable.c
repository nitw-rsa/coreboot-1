#include <console/console.h>
#include <arch/smp/mpspec.h>
#include <device/pci.h>
#include <string.h>
#include <stdint.h>

void *smp_write_config_table(void *v, unsigned long * processor_map)
{
        static const char sig[4] = "PCMP";
        static const char oem[8] = "TYAN    ";
        static const char productid[12] = "S2880       ";
        struct mp_config_table *mc;

        mc = (void *)(((char *)v) + SMP_FLOATING_TABLE_LEN);
        memset(mc, 0, sizeof(*mc));

        memcpy(mc->mpc_signature, sig, sizeof(sig));
        mc->mpc_length = sizeof(*mc); /* initially just the header */
        mc->mpc_spec = 0x04;
        mc->mpc_checksum = 0; /* not yet computed */
        memcpy(mc->mpc_oem, oem, sizeof(oem));
        memcpy(mc->mpc_productid, productid, sizeof(productid));
        mc->mpc_oemptr = 0;
        mc->mpc_oemsize = 0;
        mc->mpc_entry_count = 0; /* No entries yet... */
        mc->mpc_lapic = LAPIC_ADDR;
        mc->mpe_length = 0;
        mc->mpe_checksum = 0;
        mc->reserved = 0;

        smp_write_processors(mc, processor_map);


/*Bus:		Bus ID	Type*/
	smp_write_bus(mc, 0, "PCI   ");
	smp_write_bus(mc, 1, "PCI   ");
	smp_write_bus(mc, 2, "PCI   ");
	smp_write_bus(mc, 3, "PCI   ");
	smp_write_bus(mc, 4, "ISA   ");
/*I/O APICs:	APIC ID	Version	State		Address*/
	smp_write_ioapic(mc, 2, 0x11, 0xfec00000);
        {
                struct pci_dev *dev;
                uint32_t base;
                dev = dev_find_slot(0, PCI_DEVFN(0x1,1));
                if (dev) {
                        base = pci_read_config32(dev, PCI_BASE_ADDRESS_0);
                        base &= PCI_BASE_ADDRESS_MEM_MASK;
                        smp_write_ioapic(mc, 3, 0x11, base);
                }
                dev = dev_find_slot(0, PCI_DEVFN(0x2,1));
                if (dev) {
                        base = pci_read_config32(dev, PCI_BASE_ADDRESS_0);
                        base &= PCI_BASE_ADDRESS_MEM_MASK;
                        smp_write_ioapic(mc, 4, 0x11, base);
                }
	}
  
/*I/O Ints:	Type	Polarity    Trigger	Bus ID	 IRQ	APIC ID	PIN#
*/	smp_write_intsrc(mc, mp_ExtINT, MP_IRQ_TRIGGER_EDGE|MP_IRQ_POLARITY_HIGH, 0x4, 0x0, 0x2, 0x0);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_EDGE|MP_IRQ_POLARITY_HIGH, 0x4, 0x1, 0x2, 0x1);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_EDGE|MP_IRQ_POLARITY_HIGH, 0x4, 0x0, 0x2, 0x2);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_EDGE|MP_IRQ_POLARITY_HIGH, 0x4, 0x3, 0x2, 0x3);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_EDGE|MP_IRQ_POLARITY_HIGH, 0x4, 0x4, 0x2, 0x4);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_EDGE|MP_IRQ_POLARITY_HIGH, 0x4, 0x6, 0x2, 0x6);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_EDGE|MP_IRQ_POLARITY_HIGH, 0x4, 0x7, 0x2, 0x7);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_EDGE|MP_IRQ_POLARITY_HIGH, 0x4, 0x8, 0x2, 0x8);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_EDGE|MP_IRQ_POLARITY_HIGH, 0x4, 0xc, 0x2, 0xc);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_EDGE|MP_IRQ_POLARITY_HIGH, 0x4, 0xd, 0x2, 0xd);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_EDGE|MP_IRQ_POLARITY_HIGH, 0x4, 0xe, 0x2, 0xe);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_EDGE|MP_IRQ_POLARITY_HIGH, 0x4, 0xf, 0x2, 0xf);
	

	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_LEVEL|MP_IRQ_POLARITY_LOW, 0x3, 0x3, 0x2, 0x13);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_LEVEL|MP_IRQ_POLARITY_LOW, 0x3, 0x18, 0x2, 0x12);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_LEVEL|MP_IRQ_POLARITY_LOW, 0x3, 0x14, 0x2, 0x11);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_LEVEL|MP_IRQ_POLARITY_LOW, 0x1, 0x24, 0x3, 0x0);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_LEVEL|MP_IRQ_POLARITY_LOW, 0x1, 0x25, 0x3, 0x1);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_LEVEL|MP_IRQ_POLARITY_LOW, 0x1, 0x28, 0x3, 0x0);
	smp_write_intsrc(mc, mp_INT, MP_IRQ_TRIGGER_LEVEL|MP_IRQ_POLARITY_LOW, 0x1, 0x29, 0x3, 0x1);
/*Local Ints:	Type	Polarity    Trigger	Bus ID	 IRQ	APIC ID	PIN#*/
	smp_write_intsrc(mc, mp_ExtINT, MP_IRQ_TRIGGER_EDGE|MP_IRQ_POLARITY_HIGH, 0x0, 0x0, MP_APIC_ALL, 0x0);
	smp_write_intsrc(mc, mp_NMI, MP_IRQ_TRIGGER_EDGE|MP_IRQ_POLARITY_HIGH, 0x0, 0x0, MP_APIC_ALL, 0x1);
	/* There is no extension information... */

	/* Compute the checksums */
	mc->mpe_checksum = smp_compute_checksum(smp_next_mpc_entry(mc), mc->mpe_length);
	mc->mpc_checksum = smp_compute_checksum(mc, mc->mpc_length);
	printk_debug("Wrote the mp table end at: %p - %p\n",
		mc, smp_next_mpe_entry(mc));
	return smp_next_mpe_entry(mc);
}

unsigned long write_smp_table(unsigned long addr, unsigned long *processor_map)
{
	void *v;
	v = smp_write_floating_table(addr);
	return (unsigned long)smp_write_config_table(v, processor_map);
}
